#include"Events.h"
#include"Item.h"
#include"Util.h"

void Events::initCombat(const Enemy::TYPE eType, const bool surprise) {
    Enemy::Enemy enemy(eType, player.maxhealth, player.STR, player.level);
    combat(enemy, surprise);
}

void Events::combat(Enemy::Enemy& enemy, const bool surprised) {
    type("\nYou encounter a ", enemy.name, "!\n");
    enemy.displayStats();
    type("\nPrepare for battle!\n");
    bool first = true; // Is it the first round of combat?
    bool run   = false; // Is the player trying to run?
    uint16_t surprise = 0;
    if (player.Class == Player::ROGUE)
        surprise = (player.armor == Item::Armor::STEEL) ? 1 : randint(1, player.level + 1); // The player cannot surprise if they are wearing steel armor
    while (player.health > 0 && enemy.health > 0) { // While both the player and the enemy are alive
        if (surprised && first) {
            type("\nThe enemy surprised you!\n");
            surprise = 1;
        } else {
            // Player's turn
            while (true) {
                type ( true,
                    "\nWhat would you like to do?"
                    "\n1. Attack"
                    "\n2. Abilities"
                    "\n3. Display Stats"
                    "\n4. Display Enemy Stats"
                    "\n5. Run\n"
                );
                Choice choice;
                do choice = input("Enter choice: ");
                while (!choice.isChoice(true, "attack", 1, "abilities", 2, "display stats", 3, "display enemy stats", 4, "run", 5));

                if (choice.isChoice("attack", 1)) {
                    int damage = player.STR + randint(1, 6);  // Player's attack based on STR + a six-sided die–roll
                    enemy.health = std::max(enemy.health - damage, 0);
                    type("\nYou dealt ", damage, " damage to the ", enemy.name, "!\nIts health is now ", enemy.health, ".\n");
                } else if (choice.isChoice("abilities", 2)) {
                    if (!player.hasAbility) {
                        type("\nYou don't have any abilities.\n");
                        continue;
                    }
                    if (!player.raceAbilityReady && !player.classAbilityReady) {
                        type("\nYour abilities are currently unavailable.\nYou must sleep first.\n");
                        continue;
                    }
                    type("\nAbilities:\n");
                    std::unordered_map<uint16_t, std::string> abilities;
                    uint16_t i = 0;
                    if (player.Race == Player::DRAKONIAN && player.raceAbilityReady) {
                        abilities[i++] = "Dragon's Breath";
                        type("\t", i, ". Dragon's Breath (Drakonian)\n");
                    }
                    if (player.Class == Player::FIGHTER && player.classAbilityReady) {
                        abilities[i++] = "Second Wind";
                        type("\t", i, ". Second Wind (Fighter)\n");
                    }
                    type(++i, ". (go back)\n");
                    Choice abilityChoice;
                    uint16_t choiceNum;
                    bool isValidChoice = false;
                    do {
                        abilityChoice = input("Enter choice: ");
                        for (uint16_t j = 0; j < i && !isValidChoice; j++) {
                            isValidChoice = abilityChoice.isChoice(abilities[j], j + 1);
                            if (isValidChoice)
                                choiceNum = j;
                        }
                    } while (!(isValidChoice || abilityChoice.isChoice(true, "(go back)", i)));

                    if (abilities[choiceNum] == "Dragon's Breath") {
                        const uint16_t damage = player.STR + randint(1, 6);
                        const uint16_t burn = randint(1, 4);
                        enemy.health = std::max(enemy.health - damage - burn, 0);
                        type (
                            "\nYou take a deep breath, and with a powerful exhale, a torrent of searing flames erupts from your mouth, searing the ", enemy.name, " for ", damage, " damage!"
                            "\nThe ", enemy.name, " is burned for an additional ", burn, " damage!"
                            "\nIts health is now ", enemy.health, ".\n"
                        );
                        player.raceAbilityReady = false;
                    } else if (abilities[choiceNum] == "Second Wind") {
                        const float HEALING_MULTIPLIER = randint(10, 15) / 10.f;
                        const uint16_t healing = (player.maxhealth * HEALING_MULTIPLIER - player.health * HEALING_MULTIPLIER) / 2;
                        player.health += healing;
                        type (
                            "\nYou get a surge of adrenaline and heal ", healing, " points!"
                            "\nYour health is now ", player.health, ".\n"
                        );
                        player.classAbilityReady = false;
                    }
                    continue;
                } else if (choice.isChoice("display stats", 3)) {
                    player.displayStats();
                    continue;
                } else if (choice.isChoice("display enemy stats", 4)) {
                    enemy.displayStats();
                    continue;
                } else {
                    run = true;
                    type("You try to run away.\n");
                    if (randint(1, 10) == 1) {
                        run = false;
                        type("The ", enemy.name, " stopped you!\n");
                    }
                }
                break;
            }
        }

        if (player.Class == Player::ROGUE && first == true && surprise > 1 && enemy.name != Enemy::eType[Enemy::TRAVELER].name && run == false)
            type("You surprised the enemy!\n");
        else if (run == true) {
            type("You successfully ran away!\n");
            break;
        } else if (enemy.health > 0) {
            // Enemy's turn
            if (player.special == Item::Special::A_SHADOW && randint(1, 10) == 1) {
                type (
                    "\nAs the enemy swings to attack, their weapon passes through you, as if you weren't even there.\n"
                    "Your amulet has spared you from harm!\n"
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
        if (player.health > 0) {
            uint16_t expGain = randint(5, player.level * 8);
            type("\nYou defeated the ", enemy.name, "! You earned ", expGain, " experience points.\n");
            player.exp += expGain;
            if (enemy.name == Enemy::eType[Enemy::GOBLIN].name && randint(1, 2) == 1)
                player.initWeapon(Item::Weapon::CROSSBOW, "drop");
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

    // Generate a random number to determine the exploration event
    int eventType = randint(1, 10);

    if (eventType <= 6) { // Safe - 60%
        int event = randint(1, 7);
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
            player.health = std::min((uint16_t)(player.health + randint(5, 10)), player.maxhealth);
            break;
        case 6:
            type("You find a hidden garden with medicinal herbs. You gather some and regain health.\n");
            player.health = std::min((uint16_t)(player.health + randint(5, 15)), player.maxhealth);
            break;
        case 7:
            events.travelingTrader();
            break;
        }
    } else if (eventType <= 9) { // Risky - 30%
        int event = randint(1, 4);
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
        int event = randint(1, 2);
        switch (event) {
        case 1:
            events.enemyEncounter();
            break;
        case 2:
            events.hunterTrap();
            break;
        }
    }
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