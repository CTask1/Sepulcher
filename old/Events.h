#pragma once
#pragma warning(disable: 4244)
#include"Player.h"
#include"Util.h"
#include"Item.h"
#include"Game.h"
#include"Enemy.h"
#include"Trader.h"

void combat(Player&, Enemy::Enemy&, const bool);

class Events {
public:

    Events(Player& p) : player(p) {}

    void oldChest() { // Old Chest
        type("You find an old chest. Let's see what's inside...\n");
        int itemType = randint(1, 2);
        if (itemType == 1)
            player.initArmor(Item::Armor::IRON, "find");
        else
            player.initWeapon(Item::Weapon::MAGIC, "find", (player.Race == Player::ELF) ? 2 : 1);
    }

    void friendlyTraveler() { // Friendly Traveler
        type("You encounter a friendly traveler. They share some advice and give you a small gift.\n");
        {
            int giftType = randint(1, 2);
            if (giftType == 1) {
                type("The traveler gives you a health potion. Your health has been restored!\n");
                player.health = player.maxhealth;
            }
            else {
                int expGain = (randint(1, 100) == 1) ? randint(25, 50) : randint(5, 15);
                type("The traveler shares valuable knowledge. You gained ", expGain, " experience points!\n");
                player.exp += expGain;
            }
        }
    }

    void initCombat(const Enemy::TYPE eType, const bool surprise = false) {
        Enemy::Enemy enemy(eType, player.maxhealth, player.STR, player.level);
        combat(enemy, surprise);
    }

    void enemyEncounter() { // Enemy Encounter
        int encounter = randint(1, 3);
        switch (encounter) {
        case 1: {
            type("You stumble upon a dark cave. As you enter, you encounter a fearsome enemy!\n");
            initCombat(Enemy::SHADOW);
            break;
        } case 2: {
            type("You find a lit campfire. As you approach, you encounter a fearsome enemy!\n");
            initCombat(Enemy::GOBLIN);
            break;
        } case 3: {
            type("You venture into a cursed forest. Strange occurrences unsettle your senses.\n");
            initCombat(Enemy::SPIRIT);
            break;
        }}
    }

    void hiddenArmory() { // Hidden Armory
        type("You find a hidden armory. Let's see what equipment is available...\n");
        int itemType = randint(1, 2);
        if (itemType == 1)
            player.initArmor(Item::Armor::STEEL, "find", (player.Race == Player::HUMAN) ? 2 : 1);
        else
            player.initWeapon(Item::Weapon::GREAT, "find", (player.Class == Player::FIGHTER) ? 2 : 1);
    }

    void lostTraveler() { // Lost Traveler
        type (
            "You encounter a lost traveler who is in need of directions.\n"
            "What would you like to do?\n"
            "1. Offer help\n"
            "2. Ignore them\n"
        );
        Choice travelerChoice;
        do travelerChoice = input("Enter choice: ");
        while (!travelerChoice.isChoice(true, "offer help", 1, "ignore them", 2));
        
        if (travelerChoice.isChoice("offer help", 1)) {
            type("You offer help to the lost traveler.");
            int hostile = randint(1, 3);
            if (hostile == 1) {
                type(" As you assist the traveler, they turn around and attack you!\n");
                initCombat(Enemy::TRAVELER, true);
            } else {
                int expGain = (randint(1, 100) == 1) ? randint(35, 75) : randint(10, 20);
                type(" They are grateful and share a bit of their wisdom.\nYou gained ", expGain, " experience points!\n");
                player.exp += expGain;
            }
        } else
            type("You choose to ignore the lost traveler.\n");
    }

    void mountainPass() { // Mountain Pass
        type (
            "You come across a treacherous mountain pass. Climbing it will be challenging.\n"
            "What would you like to do?\n"
        );
        type (
            "1. Attempt the climb\n"
            "2. Find an alternate route\n"
        );
        Choice climbChoice;
        do climbChoice = input("Enter choice: ");
        while (!climbChoice.isChoice(true, "attempt the climb", 1, "find an alternate route", 2));
        
        if (climbChoice.isChoice("attempt the climb", 1)) {
            type("You decide to attempt the climb. It's steep and dangerous, but you press on.\n");
            int success = randint(1, 3);
            if (success != 1)
                type("You successfully navigate the mountain pass without incident.\n");
            else {
                type("The climb proves challenging. You slip and fall, losing some health.\n");
                player.health -= randint(int(player.maxhealth / 4), int(player.maxhealth / 3));
            }
        } else {
            type("\nYou opt to find an alternate route, avoiding the dangerous climb.\n");
            {
                int altRouteEvent = randint(1, 4);
                switch (altRouteEvent) {
                case 1: {
                    type("As you take the alternate route, you find yourself ambushed by a bandit!\n");
                    initCombat(Enemy::BANDIT, true);
                    break;
                } case 2:
                    type("You come across a group of friendly travelers along the alternate route.\n");
                    type("They share supplies and offer valuable information.\n");
                    player.receiveGift();
                    break;
                case 3: {
                    type("The alternate route leads you to a crossroads with mysterious markings. Choose a path carefully.\n");
                    type("1. Follow the left path\n2. Follow the right path\n");
                    Choice pathChoice;
                    do pathChoice = input("Enter choice: ");
                    while (!pathChoice.isChoice(true, "left", 1, "right", 2));
                    
                    if (pathChoice.isChoice("left", 1)) {
                        type("\nYou decide to follow the left path, guided by the mysterious markings.\n");
                        int outcome = randint(1, 2);
                        if (outcome == 1) {
                            type("The left path leads to a peaceful glade with a group of friendly travelers. They share supplies with you!\n");
                            player.receiveGift();
                        } else {
                            type("As you venture down the left path, you stumble upon an abandoned campsite. Investigate or continue?\n");
                            type("1. Investigate the campsite\n2. Continue on the path\n");
                            Choice campsiteChoice;
                            do campsiteChoice = input("Enter choice: ");
                            while (!campsiteChoice.isChoice(true, "investigate the campsite", 1, "continue on the path", 2));
                            
                            if (campsiteChoice.isChoice("investigate the campsite", 1)) {
                                type("\nYou decide to investigate the campsite. It seems abandoned, but you find a hidden stash of supplies!\n");
                                player.receiveGift();
                                int ambush = randint(1, 4);
                                if (ambush == 1) {
                                    type("\nAs you are leaving the campsite, you are ambushed by a bandit!\n");
                                    initCombat(Enemy::BANDIT, true);
                                }
                            } else
                                type("You choose to continue on the path, avoiding potential danger at the abandoned campsite.\n");
                        }
                    } else {
                        type("\nYou choose to follow the right path, intrigued by the mysterious markings.\n");
                        int outcome = randint(1, 2);
                        if (outcome == 1) {
                            type("\nThe right path leads to a dense forest with unique flora.\nYou collect rare herbs with medicinal properties and regain health!\n");
                            player.health = std::min((uint16_t)(player.health + randint(5, 15)), player.maxhealth);
                        } else {
                            type("\nAs you venture down the right path, you encounter a wild animal. How will you react?\n");
                            type (
                                "1. Try to calm the animal\n"
                                "2. Retreat slowly\n");
                            Choice animalChoice;
                            do animalChoice = input("Enter choice: ");
                            while (!animalChoice.isChoice(true, "try to calm the animal", 1, "retreat slowly", 2));
                            
                            if (animalChoice.isChoice("try to calm the animal", 1)) {
                                type("\nYou attempt to calm the wild animal. Surprisingly, it responds peacefully and leaves.\n");
                            } else
                                type("\nYou choose to retreat slowly, avoiding confrontation with the wild animal.\n");
                        }
                    }
                    break;
                } case 4:
                    type("You find nothing noteworthy along the alternate route.\n");
                    break;
                }
            }
        }
    }

    void mysteriousCave() { // Mysterious Cave
        type("You discover a mysterious cave entrance. It emanates an eerie glow from within.\n");
        type("What would you like to do?\n");
        Choice caveChoice;
        do caveChoice = input("1. Enter the cave\n2. Move on\nEnter choice: ");
        while (!caveChoice.isChoice(true, "enter the cave", 1, "move on", 2));
        
        if (caveChoice.isChoice("enter the cave", 1)) {
            type("You decide to enter the mysterious cave, drawn by its allure.\n");
            int encounter = randint(1, 2);
            if (encounter == 1) {
                type("The cave is filled with valuable crystals. You gain some resources!\n");
                player.resources["crystals"] += randint(1, player.level);
            }
            else {
                type("The cave is home to a hostile creature!\n");
                initCombat(Enemy::CAVE_CREATURE);
            }
        } else
            type("You choose to stay cautious and continue exploring.\n");
    }

    void strangeAmulet() { // Strange Amulet
        type (
            "You notice something glimmering beneath a pile of fallen leaves.\n"
            "Brushing away the dirt, you uncover an ornate amulet, its surface engraved with swirling patterns.\n"
            "A faint hum resonates from the artifact, and you get the feeling that it is calling to you.\n"
            "There's no telling if it's blessed or cursed. What would you like to do?\n"
            "1. Pick it up\n"
            "2. Leave it\n"
        );
        Choice choice;
        do choice = input("Enter Choice: ");
        while (!choice.isChoice(true, "pick it up", 1, "leave it", 2));

        if (choice.isChoice("pick it up", 1)) {
            uint16_t amuletType = randint(1, 5);
            switch (amuletType) {
            case 1: {
                type (
                    "The amulet pulses with powerful energy.\n"
                    "You feel stronger, faster, and ready for anything.\n"
                );
                player.initSpecial(Item::Special::A_WARBORN);
                break;
            } case 2: {
                type (
                    "A soothing presence envelops you, as if unseen hands are guiding your movements.\n"
                    "Your reflexes feel sharper, and your stance more solid.\n"
                );
                player.initSpecial(Item::Special::A_GUARDIAN);
                break;
            } case 3: {
                type (
                    "Your vision flickers, and for a moment you feel detached from reality.\n"
                    "The world seems... quieter.\n"
                );
                player.initSpecial(Item::Special::A_SHADOW);
                break;
            } case 4: {
                type (
                    "The amulet pulses, filling you with an insatiable hunger.\n"
                    "You feel stronger, but your sense of caution wanes.\n"
                );
                player.initSpecial(Item::Special::A_FURY);
                break;
            } case 5: {
                type (
                    "A sharp pain strikes your chest, and a dreadful chill spreads through your body.\n"
                    "The weight of unseen sorrow presses against your heart.\n"
                );
                player.initSpecial(Item::Special::A_WEEPING);
                break;
            }
            }
        } else {
            type (
                "You decide taking the amulet isn't worth the risk. As you turn away, a whisper drifts through the air.\n"
                "When you glance back, the amulet is gone.\n"
            );
        }
    }

    void travelingTrader() { // Traveling Trader
        type("You encounter a traveling trader!\nWould you like to browse their wares?\n");
        Choice tradeChoice;
        do tradeChoice = input("Enter choice (1. Yes / 2. No): ");
        while (!tradeChoice.isChoice(true, "yes", 1, "no", 2));

        if (tradeChoice.isChoice("yes", 1)) {
            Trader trader;
            player.resources.displayResources();
            trader.displayWares();
            while (true) {
                type(
                    "\nWhat would you like to do?"
                    "\n1. Trade"
                    "\n2. Leave\n"
                );
                Choice choice;
                do choice = input("Enter choice: ");
                while (!choice.isChoice(true, "trade", 1, "leave", 2));

                if (choice.isChoice("trade", 1)) {
                    player.resources.displayResources();
                    trader.displayWares();
                    std::string buyChoice = "";
                    do {
                        buyChoice = input("\nEnter the resource you want to trade for: ");
                        if (trader.resources[buyChoice] < 1)
                            type("The trader doesn't have that resource!\n");
                    } while (trader.resources[buyChoice] < 1);

                    bool noResources = true;
                    int amountBuy = 0;
                    do {
                        std::string strAmount = input("How many do you want? ");
                        amountBuy = (isInt(strAmount) && isPos(strAmount)) ? stoi(strAmount) : -1;
                        noResources = (amountBuy == -1 || trader.resources[buyChoice] < amountBuy);
                        if (noResources)
                            type("The trader doesn't have that many resources!\n");
                    } while (noResources);
                    
                    std::string sellChoice = "";
                    do {
                        sellChoice = input("Enter the resource you want to trade: ");
                        if (player.resources[sellChoice] < 1)
                            type("You don't have that resource!\n");
                    } while (player.resources[sellChoice] < 1);

                    int amountSell = 0;
                    do {
                        std::string strAmount = input("How many do you want to trade? ");
                        amountSell = (isInt(strAmount) && isPos(strAmount)) ? stoi(strAmount) : -1;
                        noResources = (amountSell == -1 || player.resources[sellChoice] < amountSell);
                        if (noResources)
                            type("You don't have that many resources!\n");
                    } while (noResources);

                    if (trader.resources.prices[buyChoice] * amountBuy != trader.resources.prices[sellChoice] * amountSell) {
                        type("This trade is unbalanced. Do you want to do it anyway?\n");
                        Choice confirmChoice;
                        do confirmChoice = input("Enter choice (1. Yes / 2. No): ");
                        while (!confirmChoice.isChoice(true, "yes", 1, "no", 2));

                        if (confirmChoice.isChoice("no", 2))
                            continue;
                    }

                    type("\nYou got ", amountBuy, " ", buyChoice, " from the trader for ", amountSell, " ", sellChoice, ".\n");
                    trader.resources[sellChoice] += amountSell;
                    trader.resources[buyChoice] -= amountBuy;
                    player.resources[sellChoice] -= amountSell;
                    player.resources[buyChoice] += amountBuy;
                    continue;
                } else
                    type("You leave the trader.\n");
                break;
            }
        } else
            type("You pass by the trader.\n");
    }

    void hunterTrap() { // Poacher's Snare
        int trapType = randint(1, 7);
        switch (trapType) {
        case 1: { // Rope Snare
            type (
                "You suddenly find yourself caught in a rope!"
                "\nIt quickly drags you by the foot into the air, hanging from a tree."
                "\nWhile struggling to break free, you spot movement in the bushes.\n"
            );
            wheel(3);
            bool fail = (randint(1, 3) == 1);
            type(fail ? "You failed to cut yourself down in time!" : "You successfully cut yourself out of the rope!");
            initCombat(Enemy::POACHER, fail);
            break;
        } case 2: { // Falling Net
            type (
                "You hear a quiet click underfoot."
                "\nBefore you can react, a heavy net drops onto you from above!"
                "\nFootsteps approach in the distance...whoever set this trap is nearby!\n"
            );
            wheel(3);
            bool fail = (randint(1, 3) == 1);
            type(fail ? "You failed to get out of the net in time!" : "You got out of the net before the trapper arrived!");
            initCombat(Enemy::POACHER, fail);
            break;
        } case 3: { // Swinging Log
            type (
                "You feel a tug at your ankle as a nearly invisible wire pulls taut."
                "\nWith a creaking whine, a log swings down from above, leaving you only a few moments to react!\n"
            );
            wheel(3);
            bool fail = (randint(1, 3) == 1);
            if (fail) {
                uint16_t damage = randint(1, player.health / 3);
                type("The log knocks you to the ground and you take ", damage, " damage.");
                player.health -= damage;
            } else
                type("You successfully dodge the log!\n");
            break;
        } case 4: { // Rock Avalanche
            type (
                "The ground shifts beneath your feet and a deep rumble fills the air."
                "\nBoulders begin crashing down the broken terrain toward you as dust and debris fills the air.\n"
            );
            wheel(3);
            bool fail = (randint(1, 3) == 1);
            if (fail) {
                uint16_t damage = randint(1, player.health / 3);
                type("You are knocked to the ground by a flying rock, taking ", damage, " damage.");
                player.health -= damage;
            } else
                type("You successfully evade the falling rocks!\n");
            break;
        } case 5: { // Magical Vines
            type (
                "The ground trembles and suddenly thick vines lash out from the underbrush, wrapping around your legs and arms."
                "\nThey pull tight, squeezing with unnatural strength.\n"
            );
            wheel(3);
            bool fail = (randint(1, 3) == 1);
            if (fail) {
                uint16_t damage = randint(1, player.health / 3);
                type("The thorns cut through your skin and you take ", damage, " damage.");
                player.health -= damage;
            } else
                type("With some struggle, you are able to break free from the vines.\n");
            break;
        } case 6: { // Hidden Assassin
            type (
                "You find a body slumped against a tree, clutching a small satchel."
                "\nAs you approach, an arrow flies past with a sharp whistle and embeds itself into the dirt beside you!"
                "\nShadows flicker in the trees...you are not alone."
            );
            initCombat(Enemy::POACHER);
            break;
        } case 7: { // Carnivore's Den
            type (
                "You stumble into a deep pit, landing on something hard...bones; dozens of them."
                "\nA low growl rumbles behind you as a pair of glowing eyes emerge from the darkness."
                "\nWhatever killed the owners of these bones is not done feeding."
            );
            initCombat(Enemy::BEAR);
            break;
        }}
    }

    void combat(Enemy::Enemy& enemy, const bool surprised) {
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
                if (player.special == Item::Special::A_SHADOW && randint(1, 8) == 1) {
                    type (
                        "As the enemy swings to attack, their weapon passes through you, as if you weren't even there.\n"
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
    
private:
    Player& player;
};

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