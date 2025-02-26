#include"Events.h"
#include"Item.h"
#include"Util.h"
//import Util;

void Events::initCombat(const Enemy::TYPE eType, const bool surprise) {
    Enemy::Enemy enemy(eType, player.maxHealth, player.strength, player.level);
    combat(enemy, surprise);
}

void Events::combat(Enemy::Enemy& enemy, bool surprised) {
    type("\nYou encounter a ", enemy.name, "!\n");
    enemy.displayStats();
    type("\nPrepare for battle!\n");
    bool first = true; // Is it the first round of combat?
    bool run   = false; // Is the player trying to run?
    short mirrorImage = 0;
    uint16_t surprise = 0;
    if (player.Class == Player::ROGUE)
        surprise = (player.armor == Item::TYPE::ARM_STEEL) ? 1 : randint(1, player.level + 1); // The player cannot surprise if they are wearing steel armor
    while (player.health > 0 && enemy.health > 0) { // While both the player and the enemy are alive
        if (surprised && first) {
            if (!player.arcaneEye) {
                type("\nThe enemy surprised you!\n");
                surprise = 1;
            } else {
                surprised = false;
                type("\nYour arcane eye spotted the enemy before they could strike!\n");
                continue;
            }
        } else {
            // Player's turn
            while (true) {
                type ( true,
                    "\nWhat would you like to do?"
                    "\n1. Attack"
                    "\n2. Display Stats"
                    "\n3. Display Enemy Stats"
                    "\n4. Abilities"
                    "\n5. Run\n"
                );
                Choice choice;
                do choice = input("Enter choice: ");
                while (!choice.isChoice(true, "attack", 1, "abilities", 2, "display stats", 3, "display enemy stats", 4, "run", 5));

                if (choice.isChoice("attack", 1)) {
                    int damage = player.strength + randint(1, 6);  // Player's attack based on STR + a six-sided die–roll
                    enemy.health = std::max(enemy.health - damage, 0);
                    type("\nYou dealt ", damage, " damage to the ", enemy.name, "!\nIts health is now ", enemy.health, ".\n");
                } else if (choice.isChoice("display stats", 2)) {
                    player.displayStats();
                    continue;
                } else if (choice.isChoice("display enemy stats", 3)) {
                    enemy.displayStats();
                    continue;
                } else if (choice.isChoice("abilities", 4)) {
                    if (!player.hasAbility && player.Class != Player::WIZARD) {
                        type("\nYou don't have any abilities.\n");
                        continue;
                    }
                    if (!player.raceAbilityReady && !player.classAbilityReady && player.mana == 0) {
                        type("\nYour abilities are currently unavailable.\nYou must sleep first.\n");
                        continue;
                    }
                    type("\nAbilities:\n");
                    std::unordered_map<uint16_t, std::string> abilities;
                    uint16_t abilityNum= 0;
                    if (player.Race == Player::DRAKONIAN && player.raceAbilityReady) {
                        abilities[abilityNum++] = "Dragon's Breath";
                        type("\t", abilityNum, ". Dragon's Breath (Drakonian)\n");
                    }
                    if (player.Class == Player::FIGHTER && player.classAbilityReady) {
                        abilities[abilityNum++] = "Second Wind";
                        type("\t", abilityNum, ". Second Wind (Fighter)\n");
                    }
                    if (player.Class == Player::WIZARD) {
                        type("Spells (You have ", player.mana, " mana points):\n");
                        abilities[abilityNum++] = "Fire Bolt";
                        type("\t", abilityNum, ". Fire Bolt (Wizard) - 1 MP\n");
                        abilities[abilityNum++] = "Mirror Image";
                        type("\t", abilityNum, ". Mirror Image (Wizard) - 2 MP\n");
                        abilities[abilityNum++] = "Mage Armor";
                        type("\t", abilityNum, ". Mage Armor (Wizard) - 5 MP\n");
                        if (player.weapon == Item::TYPE::WPN_ST_GUARDIAN) {
                            abilities[abilityNum++] = "Recovery";
                            type("\t", abilityNum, ". Recovery (Staff of the Guardian) - 2 MP\n");
                        }
                    }
                    type(++abilityNum, ". (go back)\n");
                    Choice abilityChoice;
                    uint16_t choiceNum = abilityNum;
                    bool isValidChoice = false;
                    do {
                        abilityChoice = input("Enter choice: ");
                        for (uint16_t i = 0; i < abilityNum && !isValidChoice; i++) {
                            isValidChoice = abilityChoice.isChoice(abilities[i], i + 1);
                            if (isValidChoice)
                                choiceNum = i;
                        }
                    } while (!(isValidChoice || abilityChoice.isChoice(true, "(go back)", abilityNum)));

                    if (abilities[choiceNum] == "Dragon's Breath") {
                        const uint16_t damage = player.strength + randint(1, 6);
                        const uint16_t burn = randint(1, 4);
                        enemy.health = std::max(enemy.health - damage - burn, 0);
                        type (
                            "\nYou take a deep breath, and with a powerful exhale, a torrent of searing flames erupts from your mouth, searing the ", enemy.name, " for ", damage, " damage!"
                            "\nThe ", enemy.name, " is burned for an additional ", burn, " damage!"
                            "\nIts health is now ", enemy.health, ".\n"
                        );
                        if (enemy.health == 0)
                            break;
                        player.raceAbilityReady = false;
                    } else if (abilities[choiceNum] == "Second Wind") {
                        const float HEALING_MULTIPLIER = randint(10, 15) / 10.f;
                        const uint16_t healing = (player.maxHealth * HEALING_MULTIPLIER - player.health * HEALING_MULTIPLIER) / 2;
                        player.health += healing;
                        type (
                            "\nYou get a surge of adrenaline and heal ", healing, " points!"
                            "\nYour health is now ", player.health, ".\n"
                        );
                        player.classAbilityReady = false;
                    } else if (abilities[choiceNum] == "Fire Bolt") {
                        if (player.mana < 1) {
                            type("\nYou don't have enough mana points!\n");
                            continue;
                        }
                        const uint16_t damage = player.strength + randint(1, 8);
                        const uint16_t burn = randint(1, 4);
                        enemy.health = std::max(enemy.health - damage - burn, 0);
                        player.mana--;
                        player.health = std::min((uint16_t)(player.health + 1), player.maxHealth);
                        type (
                            "\nYou conjure a blazing ember in your palm and hurl it forward."
                            "\nThe fire bolt streaks through the air, striking the ", enemy.name, " with a burst of flames for ", damage, " damage!"
                            "\nThe ", enemy.name, " is burned for an additional ", burn, " damage!"
                            "\nIts health is now ", enemy.health, ".\n"
                        );
                        break;
                    } else if (abilities[choiceNum] == "Mirror Image") {
                        if (player.mana < 2) {
                            type("\nYou don't have enough mana points!\n");
                            continue;
                        }
                        mirrorImage = 2;
                        player.mana -= 2;
                        player.health = std::min((uint16_t)(player.health + 2), player.maxHealth);
                        type (
                            "\nYou weave an illusion, creating shimmering duplicates of yourself."
                            "\nThey flicker and shift, making it difficult for the enemy to land its strikes.\n"
                        );
                        break;
                    } else if (abilities[choiceNum] == "Mage Armor") {
                        if (player.mageArmorDefense > 0) {
                            type("\nThis spell is already active!\n");
                            continue;
                        }
                        if (player.mana < 5) {
                            type("\nYou don't have enough mana points!\n");
                            continue;
                        }
                        player.mageArmorDefense = pow(player.level + 1, 1.2f);
                        player.defense += player.mageArmorDefense;
                        player.mana -= 5;
                        player.health = std::min((uint16_t)(player.health + 5), player.maxHealth);
                        type ("\nA protective shielding aura surrounds you, boosting your defense by ", player.mageArmorDefense, "!\n");
                        break;
                    } else if (abilities[choiceNum] == "Recovery") {
                        if (player.mana < 2) {
                            type("\nYou don't have enough mana points!\n");
                            continue;
                        }
                        const float HEALING_MULTIPLIER = randint(10, 15) / 10.f;
                        const uint16_t healing = (player.maxHealth - player.health) * HEALING_MULTIPLIER / 3;
                        player.health += healing;
                        player.mana -= 2;
                        player.health = std::min((uint16_t)(player.health + 2), player.maxHealth);
                        type (
                            "\nYou channel magical energy into a healing aura, wrapping yourself in a warm, sooting light."
                            "\nYour wounds are mended and you heal ", healing, " points!"
                            "\nYour health is now ", player.health, ".\n"
                        );
                        break;
                    }
                    continue;
                } else {
                    run = true;
                    type("You try to run away.\n");
                    if (randint(1, 10) == 1 && player.weapon != Item::TYPE::WPN_ST_SHADOW) {
                        run = false;
                        type("The ", enemy.name, " stopped you!\n");
                    }
                }
                break;
            }
        }

        if (player.Class == Player::ROGUE && first && surprise > 1 && enemy.name != Enemy::eType[Enemy::TRAVELER].name && !run)
            type("You surprised the enemy!\n");
        else if (run) {
            type("You successfully ran away!\n");
            break;
        } else if (enemy.health > 0) {
            // Enemy's turn
            if (player.special == Item::TYPE::SPL_AM_SHADOW && randint(1, 10) == 1) [[unlikely]] {
                type (
                    "\nAs the enemy swings to attack, their weapon passes through you, as if you weren't even there."
                    "\nYour amulet has spared you from harm!\n"
                );
            } else if (mirrorImage > 0 && randint(1, 2) == 1 || mirrorImage == 2) {
                mirrorImage--;
                type (
                    "\nThe enemy's precise attack finds its mark, only for the image to shimmer and dissolve."
                    "\nThey stare in confusion, momentarily thrown off their attack.\n"
                );
            } else {
                uint16_t enemyDamage = std::max(enemy.attack + randint(1, 6) - player.CON - player.defense, 0);  // Enemy's attack based on its attack stat + a six-sided die roll - player's CON and defense
                enemyDamage = (enemyDamage > 0) ? enemyDamage : 0;
                player.health = std::max(player.health - enemyDamage, 0);
                type("\nThe ", enemy.name, " dealt ", enemyDamage, " damage to you!\nYour health is now ", player.health, ".\n");
            }
        }

        first = false;
    }

    // Check the outcome of the battle
    if (run == false) {
        if (player.health > 0) [[likely]] {
            uint16_t expGain = randint(5, player.level * 8);
            type("\nYou defeated the ", enemy.name, "! You earned ", expGain, " experience points.\n");
            player.exp += expGain;
            if (enemy.name == Enemy::eType[Enemy::GOBLIN].name && randint(1, 2) == 1)
                player.initWeapon(Item::TYPE::WPN_CROSSBOW, Item::Source::DROP, (player.Class == Player::WIZARD ? -1 : 1));
        } else {
            type("\nYou were defeated by the ", enemy.name, ". Game over.\n\n");
            std::cout << "Press any key to continue . . .";
            std::cin.get();
            exit(0);
        }
    }
}

void explore(Player& player) {
    type("\nYou decide to explore the surroundings...\n");

    Events events(player);

    uint16_t eventType,
             prevEvent = 0,
             event;

    do {
        // Generate a random number to determine the exploration event
        eventType = randint(1, 10);

        if (eventType <= 6) { // Safe - 60%
            event = randint(1, 7);
            if (event == prevEvent)
                continue;
            prevEvent = event;
            switch (event) {
            case 1:
                events.friendlyTraveler();
                break;
            case 2:
                events.oldChest();
                break;
            case 3:
                events.hiddenArmory();
                break;
            case 4:
                type("You found nothing worth taking note of.\n");
                break;
            case 5:
                type("You discover a peaceful meadow. The serene environment helps you relax.\n");
                player.health = std::min((uint16_t)(player.health + randint(5, 10)), player.maxHealth);
                if (player.Class == Player::WIZARD && player.mana != player.maxMana)
                    player.mana++;
                break;
            case 6:
                type("You find a hidden garden with medicinal herbs. You gather some and regain health.\n");
                player.health = std::min((uint16_t)(player.health + randint(5, 15)), player.maxHealth);
                if (player.Class == Player::WIZARD && player.mana != player.maxMana)
                    player.mana++;
                break;
            case 7:
                events.travelingTrader();
                break;
            }
        } else if (eventType <= 9) { // Risky - 30%
            event = randint(1, 4);
            if (event == prevEvent)
                continue;
            prevEvent = event;
            switch (event) {
            case 1:
                events.lostTraveler();
                break;
            case 2:
                events.mountainPass();
                break;
            case 3:
                events.mysteriousCave();
                break;
            case 4:
                events.strangeAmulet();
            }
        } else { // Dangerous - 10%
            event = randint(1, 2);
            if (event == prevEvent)
                continue;
            prevEvent = event;
            switch (event) {
            case 1:
                events.enemyEncounter();
                break;
            case 2:
                if (player.arcaneEye) {
                    type (
                        "\nYour arcane eye reveals a trap hidden in the underbrush."
                        "\nYou halt just in time, avoiding danger.\n"
                    );
                    break;
                }
                events.hunterTrap();
                break;
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