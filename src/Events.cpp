//CTask
#include<string>

#include"TextManager.h"
#include"Choice.h"
#include"Events.h"
#include"Player.h"
#include"Enemy.h"
#include"Item.h"
#include"util.h"

void playerTurn(Player& player, Enemy::Enemy& enemy, bool& run, short& mirrorImage, bool& shadowmeld, const bool sepulcher) {
    while (true) {
        type(TM::get("combat.player_turn.choose_option"));
        setMode(LIST_OUT);
        type(TM::getAllAsStr("combat.player_turn.options", {
            .replacements = {{"{potions}", std::to_string(player.resources["Health Potion"])}}
        }));

        switch (Choice::getChoice(TM::getAllAsLst("combat.player_turn.options"))) {
        case 1: {
            int damage = player.strength + randint(1, 6);  // Player's attack based on strength + a six-sided die–roll
            if (player.weapon.itemType != Item::TYPE::NONE && player.weapon.suffix == Item::Weapon::Suffix::INFERNO) {
                damage += randint(1, 4);
                type(TM::get("items.weapons.infernal"));
            }
            enemy.health = ui16(std::max(enemy.health - damage, 0));
            type(TM::get("combat.player_turn.damage_enemy", {
                .replacements = {
                    {"{damage}", std::to_string(damage)},
                    {"{enemy}", std::string(enemy.name)},
                    {"{health}", std::to_string(enemy.health)}
                }
            }));
            break;
        } case 2:
            player.displayStats();
            continue;
        case 3:
            enemy.displayStats();
            continue;
        case 4:
            if (player.abilities(&enemy, &mirrorImage, &shadowmeld))
                break;
            continue;
        case 5:
            if (player.resources["Health Potion"] < 1) {
                type(TM::get("player.health_potion.none"));
                continue;
            }
            player.resources["Health Potion"]--;
            player.heal(1);
            type(TM::get("player.health_potion.use", {
                .replacements = {{"{health}", std::to_string(player.health)}}
            }));
            continue;
        case 6:
            if (Choice::getChoice({"yes", "no"}, TM::get("confirmation", {
                .replacements = {{"{action}", "run"}},
                .end = ' '
            }).c_str()) == 2)
                continue;

            run = true;
            type(TM::get("combat.player_turn.run.message"));
            if (randint(1, (sepulcher ? 5 : 10)) == 1 && player.weapon != Item::TYPE::WPN_ST_SHADOW) {
                run = false;
                type(TM::get("combat.player_turn.run.fail", {
                    .replacements = {{"{enemy}", std::string(enemy.name)}}
                }));
            }
        }
        break;
    }
}

void enemyTurn(Player& player, Enemy::Enemy& enemy, short& mirrorImage, bool& shadowmeld) {
    if (player.special == Item::TYPE::SPL_AM_SHADOW && randint(1, 10) == 1) [[unlikely]] {
        type(TM::get("combat.enemy_turn.attack.fail.amulet"));
    } else if (mirrorImage > 0 && randint(1, 2) == 1 || mirrorImage == 2) [[unlikely]] {
        mirrorImage--;
        type(TM::get("combat.enemy_turn.attack.fail.mirror_image"));
    } else if (shadowmeld) [[unlikely]] {
        shadowmeld = false;
        type(TM::get("combat.enemy_turn.attack.fail.shadowmeld"));
    } else {
        uint16_t enemyDamage = ui16(std::max(enemy.strength + randint(1, 6) - player.CON - player.defense, 0));  // Enemy's attack based on its attack stat + a six-sided die roll - player's CON and defense
        enemyDamage = (enemyDamage > 0) ? enemyDamage : 0;
        player.health = ui16(std::max(player.health - enemyDamage, 0));
        type(TM::get("combat.enemy_turn.attack.damage_player", {
            .replacements = {
                {"{enemy}", std::string(enemy.name)},
                {"{damage}", std::to_string(enemyDamage)},
                {"{health}", std::to_string(player.health)}
            }
        }));
        if (player.armor.suffix == Item::Armor::Suffix::THORNS) {
            uint16_t thornsDamage = ui16(ceil(enemyDamage / 10.0));
            enemy.health = ui16(std::max(enemy.health - thornsDamage, 0));
            type(TM::get("combat.enemy_turn.attack.thorns", {
                .replacements = {
                    {"{damage}", std::to_string(thornsDamage)},
                    {"{enemy}", std::string(enemy.name)},
                    {"{health}", std::to_string(enemy.health)}
                }
            }));
        }
    }
}

Enemy::Enemy Events::initEnemy(const Enemy::TYPE eType, const bool boss) {
    return Enemy::Enemy(eType, player.maxHealth, player.baseStrength, player.level, boss);
}

bool Events::combat(Enemy::Enemy&& enemy, bool surprised, const bool sepulcher, const char* message) {
    const std::string defMessage = TM::get("combat.default_message", {
        .replacements = {{"{enemy}", std::string(enemy.name)}}
    });
    if (message == nullptr)
        message = defMessage.c_str();
    type('\n' + std::string(message) + '\n');
    enemy.displayStats();
    type(TM::get("combat.prepare"));
    bool first = true; // Is it the first round of combat?
    bool run   = false; // Is the player trying to run?
    short mirrorImage = 0;
    bool shadowmeld = false;
    uint16_t surprise = 0;
    if (player.Class == Player::CLASS::ROGUE)
        surprise = (player.armor == Item::TYPE::ARM_STEEL || player.armor == Item::TYPE::ARM_IRON) ? 1 : randint(1, player.level + 1); // The player cannot surprise if they are wearing heavy armor
    while (true) {
        if (surprised && first) { // If the player is surprised
            if (player.arcaneEye) { // If the player has the Arcane Eye spell active
                surprised = false;
                type(TM::get("combat.surprise.enemy.fail.arcane_eye"));
                continue;
            }
            type(TM::get("combat.surprise.enemy.success"));
            surprise = 1;
        } else
            playerTurn(player, enemy, run, mirrorImage, shadowmeld, sepulcher);

        if (player.Class == Player::CLASS::ROGUE && first && surprise > 1 && enemy.name != Enemy::eType[Enemy::TRAVELER].name && !run)
            type(TM::get("combat.surprise.player"));
        else if (run) {
            type(TM::get("combat.player_turn.run.success"));
            return 0;
        } else if (enemy.health > 0) {
            // Enemy's turn
            enemyTurn(player, enemy, mirrorImage, shadowmeld);
        }

        first = false;
        
        if (player.health <= 0 || enemy.health <= 0) {
            // Check the outcome of the battle
            if (run == false) {
                if (player.health > 0) [[likely]] {
                    uint16_t expGain = randint(5, player.level * 8);
                    type(TM::get("combat.victory", {
                        .replacements = {
                            {"{enemy}", std::string(enemy.name)},
                            {"{exp}", std::to_string(expGain)}
                        }
                    }));
                    player.addExp(expGain);
                    if (enemy == Enemy::GOBLIN && randint(1, 2) == 1)
                        player.initWeapon(Item::TYPE::WPN_CROSSBOW, Item::Source::DROP, (player.Class == Player::CLASS::WIZARD ? -1 : 1));
                    return 1;
                } else {
                    if (checkDeath(player, enemy))
                        return 0;
                }
            }
        }
    }
    return 1;
}

bool checkDeath(Player& player, Enemy::Enemy& enemy) {
    if (player.health <= 0) {
        type(TM::get("combat.defeat", {
            .replacements = {
                {"{enemy}", std::string(enemy.name)},
                {"{rev}", TM::getForCondition("combat.resurgence", player.resurgence)}
            }
        }));
        if (player.resurgence) {
            player.health = player.getMaxHealth() / 2;
            player.resurgence = false;
            return 0;
        }
        return 1;
    }
    return 0;
}

void explore(Player& player) {
    type(TM::get("events.explore"));

    static Events events(player);
    static uint16_t prevEvent = 0;
    uint16_t eventType,
             event;

    do {
        // Generate a random number to determine the exploration event
        eventType = randint(1, 10);

        if (eventType <= (day ? 6 : 2)) { // Safe - 60% / 20%
            // at night, traveling trader is not available, and friendly traveler is less likely
            // if statue has been found, it cannot be found again
            event = randint((day ? 1 : randint(2, 3)), (statueFound ? 8 : 9));
            if (event == prevEvent)
                continue;
            prevEvent = event;
            switch (event) {
                case 1: events.travelingTrader(); break;
                case 2: events.friendlyTraveler(); break;
                case 3: events.oldChest(); break;
                case 4: events.hiddenArmory(); break;
                case 5: events.nothing(); break;
                case 6: events.meadow(); break;
                case 7: events.stream(); break;
                case 8: events.garden(); break;
                case 9: events.statue(); break;
            }
        } else if (eventType <= (day ? 9 : 5)) { // Risky - 30%
            event = randint(1, 4);
            if (event == prevEvent)
                continue;
            prevEvent = event;
            switch (event) {
                case 1: events.lostTraveler(); break;
                case 2: events.mountainPass(); break;
                case 3: events.mysteriousCave(); break;
                case 4: events.strangeAmulet(); break;
            }
        } else { // Dangerous - 10% / 50%
            // at night, hunter's trap is less likely
            // sepulcher can only be encountered at level 10 or higher
            event = randint((day ? 1 : randint(1, 2)), (player.level >= 10 ? 3 : 2));
            if (event == prevEvent)
                continue;
            prevEvent = event;
            switch (event) {
                case 1: events.hunterTrap(); break;
                case 2: events.enemyEncounter(); break;
                case 3: events.sepulcher(); break;
            }
        }
        break;
    } while (true);
    /* Other events
     * Risky:
     *   Enchanted fountain -- Drink from it? It could grant a buff...or a curse
     *   Talking statue -- Answer its riddle...or else
     *   Treasure chest -- It could have an item...or it could be a mimic
     *   Whispering shadows -- It could lead to treasure, a powerful artifact, or an encounter with a dark shadow.
     *   Armor stand -- A beautiful set of armor. It could be possessed.
     *   Suspicious statue -- A statue holds a magical weapon. Will it defend itself?
     * Dangerous:
     *   Apex predator -- Find a massive, rare beast. Approach?
     *   Night only:
     *     Blood moon -- All enemies are stronger and encounters are more common
     */
}