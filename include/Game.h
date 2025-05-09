//CTask
#pragma once
#include<vector>

#include"Choice.h"
#include"Events.h"
#include"Player.h"
#include"settings.h"
#include"globals.h"
#include"util.h"

void resetStats(Player& player, uint16_t& time, uint16_t& energy) {
    player.defense -= player.mageArmorDefense;
    player.mageArmorDefense = 0;
    player.raceAbilityReady = true;
    player.classAbilityReady = true;
    player.arcaneEye = false;
    time = DAY_LENGTH;
    energy = DAY_LENGTH + 1;
}

void rest(Player& player, uint16_t& energy) {
    type("\nYou find a place to rest and gain some health.\n");
    player.heal();
    if (player.Class == Player::CLASS::WIZARD && player.mana != player.maxMana)
         player.mana++;
    energy++;
}

bool useHealthPotion(Player& player) {
    if (player.resources["Health Potion"] < 1) {
        type("\nYou don't have any health potions!\n");
        return false;
    }
    player.resources["Health Potion"]--;
    player.heal(1);
    type("\nYou used a health potion!\nYour health is now full (", player.health, ").\n");
    return true;
}

void otherOptions(Player& player) {
    while (true) {
        setList(true);
        type (
            "\nOther options:\n"
            "\t1. Use a health potion (", player.resources["Health Potion"], ")\n"
            "\t2. Unequip armor\n"
            "\t3. Unequip weapon\n"
            "\t4. Abilities\n",
            (player.Class == Player::CLASS::WIZARD ? "\t5. Rituals\n6" : "5"),
            ". (go back)\n"
        );

        std::vector<const char*> choices {
            "use a health potion",
            "unequip armor",
            "unequip weapon",
            "abilities"
        };
        if (player.Class == Player::CLASS::WIZARD)
            choices.push_back("rituals");
        choices.push_back("(go back)");

        int optionsChoice = 0;
        do optionsChoice = Choice(input(prompt.data())).isChoice(choices);
        while (optionsChoice == 0);
        
        switch (optionsChoice) {
            case 1: if (!useHealthPotion(player)) continue; break;
            case 2: player.unequipArmor(); continue;
            case 3: player.unequipWeapon(); continue;
            case 4: if (!player.abilities()) continue; break;
            case 5: if (player.Class == Player::CLASS::WIZARD && !player.rituals()) continue; break;
        }
        break;
    }
}

bool quit() {
    int quitChoice;
    do quitChoice = Choice(input("Are you sure you want to quit (1. Yes / 2. No)? ")).isChoice({"yes", "no"});
    while (quitChoice == 0);
    if (quitChoice == 1)
        return true;
    return false;
}

void gameLoop(Player& player, uint16_t hitdie) {
    uint16_t time = DAY_LENGTH;
    uint16_t energy = DAY_LENGTH;
    // Main game loop
    while (true) {
        if (day && energy == 0 || !day && energy == 1)
            type ("\nYour vision blurs. You can't go on much longer...\n");
        if (!day) {
            if (energy == 0) {
                setDelay(20);
                type (
                    "\nYour legs give out beneath you."
                    "\nThe world spins, and your vision narrows to a tunnel of darkness."
                    "\nYou collapse where you stand, claimed by exhaustion.\n"
                );
                player.addDebuff(Debuff::EXHAUSTED);
                resetStats(player, time, energy);
                day = true;
                setDelay(20);
                type (
                    "\nYou wake on cold, unforgiving ground, limbs aching and breath shallow."
                    "\nThe sun has risen, but its warmth does nothing for the weight pressing on your chest."
                    "\nYou're alive—but just barely. Exhaustion clings to you like a second skin.\n"
                );
            }
        }
        if (time == 0) [[unlikely]] { // if the day/night is over
            if (day) {
                type (
                    "\nTwilight drapes the land in shadow, and unseen movement whispers in the distance."
                    "\nNight has fallen. You can sleep now... or face what hunts in the dark."
                    "\nWhat would you like to do?"
                    "\n1. Sleep"
                    "\n2. Continue", (player.level < 5 || energy < 4) ? " (not recommended)\n" : "\n"
                );
                int nightChoice = 0;
                do nightChoice = Choice(input(prompt.data())).isChoice({"sleep", "continue"});
                while (nightChoice == 0);
    
                switch (nightChoice) {
                case 1:
                    type (
                        "\nYou find a safe place to make camp and sleep through the night.\n",
                        (player.Race == Player::RACE::REVENANT ? "Another restless night passes" : "Your health has been restored"),
                        (player.hasAbility ? " and your abilities have recharged!" : "!"),
                        (player.Class == Player::CLASS::WIZARD ? "\nYou also recovered 5 mana points!\n" : "\n")
                    );
                    if (player.Race == Player::RACE::REVENANT) {
                        if (player.bloodMeter < 3) {
                            type ("\nYou were unable to fill your blood meter!\n");
                            player.addDebuff(Debuff::RAVENOUS);
                        }
                        player.bloodMeter = 0;
                        type("\nYour blood meter has been emptied.\n");
                    } else
                        player.healMax();
                    
                    player.removeDebuff(Debuff::EXHAUSTED);
                    resetStats(player, time, energy);
                    player.mana = std::min(player.maxMana, (uint16_t)(player.mana + 5));
                    break;
                case 2:
                    type("You decide to continue on into the night. Good luck!\n");
                    day = false;
                    time = NIGHT_LENGTH;
                    break;
                }
            } else {
                type (
                    "\nThe darkness lifts as the first light of dawn touches the horizon."
                    "\nThe weariness in your limbs fades with the morning light. A new day begins.\n"
                );
                resetStats(player, time, energy);
                day = true;
            }
            
        } else {
            setList(true);
            type (
                "\nWhat would you like to do?"
                "\n1. Explore"
                "\n2. Display Stats"
                "\n3. Gather Resources"
                "\n4. Craft"
                "\n5. Rest"
                "\n6. Other options"
                "\n7. Settings"
                "\n8. Quit\n"
            );
            int choice = 0;
            do choice = Choice(input(prompt.data())).isChoice({"explore", "display stats", "gather resources", "craft", "rest", "other options", "settings", "quit"});
            while (choice == 0);
            
            switch (choice) {
                case 1: explore(player); break;
                case 2: player.displayStats(true); continue;
                case 3: player.gatherResources(); break;
                case 4: player.craft(); continue;
                case 5: rest(player, energy); break;
                case 6: otherOptions(player); continue;
                case 7: settings(); continue;
                case 8: if (quit()) return;
            }

            if (player.health <= 0) {
                type("\nYou have died. Game over.\n\n");
                return;
            }
            time--;
            energy--;
            player.levelUp(hitdie);
            player.updateDebuffs();
        }
    }
}

void start() {
    Player::RACE pRace = Player::RACE::ELF;
    Player::CLASS pClass = Player::CLASS::FIGHTER;
    uint16_t hitdie = 0;
    uint16_t str = 0, con = 0, def = 0;
    
    setOutputSettings();
    { // Get the player's choice of race
        type (
            "Hello! Welcome to the game.\nPlease select a race for your character:"
            "\n1. Elf       - Elves are a magical people with strong ties to nature. They are proficient with magical items."
            "\n2. Human     - Humans are adaptable and resilient, thriving in any environment. They are proficient with heavy weapons and armor."
            "\n3. Drakonian - Drakonians are a proud, ancient race born from the blood of dragons. They are known for their great strength and fiery breath."
            "\n4. Revenant  - Revenants are restless souls bound to the mortal plane. They endure with unnatural vitality but must feed on the life force of others.\n"
        );
        int raceChoice = 0;
        do raceChoice = Choice(input(prompt.data())).isChoice({"elf", "human", "drakonian", "revenant"});
        while (raceChoice == 0);
        
        switch (raceChoice) {
        case 1:
            pRace = Player::RACE::ELF;
            con = 3;
            break;
        case 2:
            pRace = Player::RACE::HUMAN;
            con = 3;
            break;
        case 3:
            pRace = Player::RACE::DRAKONIAN;
            con = 2;
            def = 1;
            break;
        case 4:
            pRace = Player::RACE::REVENANT;
            con = 3;
            break;
        }
    }
    
    { // Get the player's choice of class
        type (
            "\nNow select a class:"
            "\n1. Fighter   - Trained with blades of all sorts      - Difficulty: Medium"
            "\n2. Rogue     - A silent assassin, skilled in stealth - Difficulty: Low"
            "\n3. Wizard    - A scholar in the arcane arts          - Difficulty: High\n"
        );
        int classChoice = 0;
        do classChoice = Choice(input(prompt.data())).isChoice({"fighter", "rogue", "wizard"});
        while (classChoice == 0);
        
        switch (classChoice) {
        case 1:
            pClass = Player::CLASS::FIGHTER;
            hitdie = 16;
            str = 4;
            break;
        case 2:
            pClass = Player::CLASS::ROGUE;
            hitdie = 14;
            str = 4;
            break;
        case 3:
            pClass = Player::CLASS::WIZARD;
            hitdie = 12;
            str = 5;
            break;
        }
    }

    std::string pName = input("\nPlease enter a name for your character: ");
    
    type("\nNice to meet you, ", pName, ". Here are your stats:\n");
    Player player(pName, pRace, pClass, hitdie + con, str, con);
    #if DEV_MODE
    player.initWeapon(Item::Weapon(Item::TYPE::WPN_GREAT, 10, 1, true), Item::Source::FIND);
    player.initArmor(Item::Armor(Item::TYPE::ARM_STEEL, 10, 1, true), Item::Source::FIND);
    player.addExp(500);
    player.levelUp(hitdie);
    player.resources.addResource("Health Potion", 10);
    #endif
    player.defense += def;
    gameLoop(player, hitdie);
}