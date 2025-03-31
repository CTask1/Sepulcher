#pragma once
#include<string>
#include<cmath>

#include"Events.h"
#include"Player.h"
#include"Util.h"
#include"consts.h"

void gameLoop(Player& player, uint16_t hitdie) {
    uint16_t time = DAY_LENGTH;
    uint16_t energy = DAY_LENGTH;
    // Main game loop
    while (true) {
        Choice choice;
        if (time == 0) [[unlikely]] {
            type (
                "\nAs the sun dips below the horizon, you find a safe place to make camp and sleep through the night."
                "\nYour health has been restored", (player.hasAbility ? " and your abilites have recharged!" : "!"),
                (player.Class == Player::WIZARD ? "\nYou also recovered 5 mana points!\n" : "\n")
            );
            /*type (
                "\nIt's getting late. You should get some sleep...unless you want to challenge the darkness."
                "\nWhat would you like to do?"
                "\n1. Sleep"
                "\n2. Continue", (player.level < 5 || energy < 4) ? " (not recommended)\n" : "\n"
            );
            do choice = input("Enter choice: ");
            while (!choice.isChoice(true, "sleep", 1, "continue", 2));*/

            //if (choice.isChoice("sleep", 1)) {
                //type("\nYou find a place to sleep through the night.\n");
                if (player.Race == Player::REVENANT) {
                    if (player.bloodMeter < 3) {
                        type ("\nYou were unable to fill your blood meter!\n");
                        player.addDebuff(Debuff::RAVENOUS);
                    }
                    player.bloodMeter = 0;
                    type("\nYour blood meter has been emptied.\n");
                } else
                    player.healMax();
                    
                player.defense -= player.mageArmorDefense;
                player.mageArmorDefense = 0;
                player.mana = std::min(player.maxMana, (uint16_t)(player.mana + 5));
                player.raceAbilityReady = true;
                player.classAbilityReady = true;
                player.arcaneEye = false;
                time = energy = DAY_LENGTH;
            //} else 
            //    type("You decide to continue on into the night. Good luck!\n");
        } else {
            setOutputSettings(true);
            type (
                "\nWhat would you like to do?"
                "\n1. Explore"
                "\n2. Display Stats"
                "\n3. Gather Resources"
                "\n4. Craft"
                "\n5. Rest"
                "\n6. Other options"
                "\n7. Quit\n"
            );
            do choice = input("Enter choice: ");
            while (!choice.isChoice(true, "explore", 1, "display stats", 2, "gather resources", 3, "craft", 4, "rest", 5, "other options", 6, "quit", 7));
            
            if (choice.isChoice("explore", 1)) {
                explore(player);
            } else if (choice.isChoice("display stats", 2)) {
                player.displayStats();
                player.resources.displayResources();
                continue;
            } else if (choice.isChoice("gather resources", 3)) {
                player.gatherResources();
            } else if (choice.isChoice("craft", 4)) {
                player.craft();
                continue;
            } else if (choice.isChoice("rest", 5)) {
                type("\nYou find a place to rest and gain some health.\n");
                player.heal();
                if (player.Class == Player::WIZARD && player.mana != player.maxMana)
                    player.mana++;
               energy++;
            } else if (choice.isChoice("other options", 6)) {
                while (true) {
                    std::unordered_map<uint16_t, std::string> options;
                    uint16_t optionNum = 0;
                    setOutputSettings(true);
                    type (
                        "\nOther options:\n"
                        "\t1. Unequip armor\n"
                        "\t2. Unequip weapon\n"
                        "\t3. Abilities\n"
                    );
                    options[optionNum++] = "Unequip armor";
                    options[optionNum++] = "Unequip weapon";
                    options[optionNum++] = "Abilities";
                    if (player.Class == Player::WIZARD) {
                        options[optionNum++] = "Rituals";
                        type("\t", optionNum, ". Rituals\n");
                    }
                    type(++optionNum, ". (go back)\n");

                    Choice optionsChoice;
                    uint16_t optionsChoiceNum = optionNum;
                    bool isValidOptionChoice = false;
                    do {
                        optionsChoice = input("Enter choice: ");
                        for (uint16_t i = 0; i < optionNum && !isValidOptionChoice; i++) {
                            isValidOptionChoice = optionsChoice.isChoice(options[i], i + 1);
                            if (isValidOptionChoice)
                                optionsChoiceNum = i;
                        }
                    } while (!(isValidOptionChoice || optionsChoice.isChoice(true, "(go back)", optionNum)));

                    if (options[optionsChoiceNum] == "Unequip armor") {
                        player.unequipArmor();
                        continue;
                    } else if (options[optionsChoiceNum] == "Unequip weapon") {
                        player.unequipWeapon();
                        continue;
                    } else if (options[optionsChoiceNum] == "Abilities") {
                        if (!player.abilities())
                            continue;
                    } else if (options[optionsChoiceNum] == "Rituals") {
                        if (!player.rituals())
                            continue;
                    }
                    break;
                }
                continue;
            } else {
                Choice quitChoice;
                do quitChoice = input("Are you sure you want to quit (1. Yes / 2. No)? ");
                while (!quitChoice.isChoice(true, "yes", 1, "no", 2));
                if (quitChoice.isChoice("yes", 1))
                    return;
            }

            if (player.health <= 0) {
                type("\nYou have died. Game over.\n\n");
                return;
            }
            time--;
            energy--;
            levelUp(player, hitdie);
            player.updateDebuffs();
        }
    }
}

void start() {
    Player::RACE pRace;
    Player::CLASS pClass;
    uint16_t hitdie = 0;
    uint16_t str, con, def = 0;
    
    setOutputSettings();
    { // Get the player's choice of race
        type (
            "Hello! Welcome to the game.\nPlease select a race for your character:"
            "\n1. Elf       - Elves are a magical people with strong ties to nature. They are proficient with magical items."
            "\n2. Human     - Humans are adaptable and resilient, thriving in any environment. They are proficient with heavy weapons and armor."
            "\n3. Drakonian - Drakonians are a proud, ancient race born from the blood of dragons. They are known for their great strength and fiery breath."
            "\n4. Revenant  - Revenants are restless souls bound to the mortal plane. They endure with unnatural vitality but must feed on the life force of others.\n"
        );
        Choice raceChoice;
        do raceChoice = input("Please enter your choice: ");
        while (!raceChoice.isChoice(true, "elf", 1, "human", 2, "drakonian", 3, "revenant", 4));
        
        if (raceChoice.isChoice("elf", 1)) {
            pRace = Player::ELF;
            con = 3;
        } else if (raceChoice.isChoice("human", 2)) {
            pRace = Player::HUMAN;
            con = 3;
        } else if (raceChoice.isChoice("drakonian", 3)) {
            pRace = Player::DRAKONIAN;
            con = 3;
            def = 1;
        } else {
            pRace = Player::REVENANT;
            con = 3;
        }
    }
    
    { // Get the player's choice of class
        type (
            "\nNow select a class:"
            "\n1. Fighter   - Trained with blades of all sorts      - Difficulty: Medium"
            "\n2. Rogue     - A silent assassin, skilled in stealth - Difficulty: Low"
            "\n3. Wizard    - A scholar in the arcane arts          - Difficulty: High\n"
        );
        Choice classChoice;
        do classChoice = input("Please enter your choice: ");
        while (!classChoice.isChoice(true, "fighter", 1, "rogue", 2, "wizard", 3));
        
        if (classChoice.isChoice("fighter", 1)) {
            pClass = Player::FIGHTER;
            hitdie = 16;
            str = 4;
        } else if (classChoice.isChoice("rogue", 2)) {
            pClass = Player::ROGUE;
            hitdie = 14;
            str = 4;
        } else {
            pClass = Player::WIZARD;
            hitdie = 12;
            str = 4;
        }
    }

    std::string pName = input("\nPlease enter a name for your character: ");
    
    type("\nNice to meet you, ", pName, ". Here are your stats:\n");
    Player player(pName, pRace, pClass, hitdie + con, str, con);
    player.defense += def;
    gameLoop(player, hitdie);
}