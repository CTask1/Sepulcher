#include"..\include\Events.h"
#include"..\include\Item.h"
#include"..\include\Util.h"
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
        surprise = (player.armor == Item::TYPE::ARM_STEEL || player.armor == Item::TYPE::ARM_IRON) ? 1 : randint(1, player.level + 1); // The player cannot surprise if they are wearing steel armor
    while (player.health > 0 && enemy.health > 0) {
        if (surprised && first) { // If the player is surprised
            if (player.arcaneEye) { // If the player has the Arcane Eye spell active
                surprised = false;
                type("\nYour arcane eye spotted the enemy before they could strike!\n");
                continue;
            }
            type("\nThe enemy surprised you!\n");
            surprise = 1;
        } else {
            // Player's turn
            while (true) {
                setOutputSettings(true);
                type (
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
                    int damage = player.strength + randint(1, 6);  // Player's attack based on strength + a six-sided die–roll
                    enemy.health = std::max(enemy.health - damage, 0);
                    type("\nYou dealt ", damage, " damage to the ", enemy.name, "!\nIts health is now ", enemy.health, ".\n");
                } else if (choice.isChoice("display stats", 2)) {
                    player.displayStats();
                    continue;
                } else if (choice.isChoice("display enemy stats", 3)) {
                    enemy.displayStats();
                    continue;
                } else if (choice.isChoice("abilities", 4)) {
                    if (player.abilities(&enemy, &mirrorImage))
                        break;
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
        eventType =  randint(1, 10);

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
                player.heal(3);
                if (player.Class == Player::WIZARD && player.mana != player.maxMana)
                    player.mana++;
                break;
            case 6:
                type("You find a hidden garden with medicinal herbs. You gather some and regain health.\n");
                player.heal(3);
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
            event = randint(1, 2); // ((player.level >= 10 ? 3 : 2));
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
            case 3:
                events.sepulcher();
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