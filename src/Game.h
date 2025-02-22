#pragma once
#include<cmath>
#include<string>
#include"Events.h"
#include"Player.h"
#include"Item.h"
#include"Util.h"
#include"Enemy.h"

void levelUp(Player& player, int hitdie) {
    uint16_t levels = 0;
    while (player.exp >= player.nextLevel) {
        player.level++;
        player.exp -= player.nextLevel;
        player.maxhealth += player.level + hitdie;
        player.health = player.maxhealth;
        player.STR += 3;
        player.baseSTR += 3;
        if (player.Race == Player::DRAKONIAN)
            player.defense += 1;
        player.nextLevel = 10 + static_cast<uint32_t>(pow(player.level, 2));
        levels++;
    }
    if (levels > 0) {
        type("You leveled up");
        if (levels == 2)
            type(" twice");
        else if (levels == 3)
            type(" three times");
        type("!\n");
    }
}

void gameLoop(Player& player, int hitdie) {
    uint16_t time = 5;
    uint16_t energy = 5;
    // Main game loop
    while (true) {
        Choice choice;
        if (time == 0) [[unlikely]] {
            type (
                "\nAs the sun dips below the horizon, you find a safe place to make camp and sleep through the night."
                "\nYour health has been restored", (player.hasAbility ? " and your abilites have recharged!\n" : "!\n")
            );
            /*type (
                "\nIt's getting late. You should get some sleep...unless you want to challenge the darkness."
                "\nWhat would you like to do?"
                "\n1. Sleep"
                "\n2. Continue", (player.level < 5 || energy < 3) ? " (not recommended)\n" : "\n"
            );
            do choice = input("Enter choice: ");
            while (!choice.isChoice(true, "sleep", 1, "continue", 2));*/

            //if (choice.isChoice("sleep", 1)) {
                //type("\nYou find a place to sleep through the night.\n");
                player.health = player.maxhealth;
                player.raceAbilityReady = true;
                player.classAbilityReady = true;
                time = 5;
                energy = 5;
            //} else 
            //    type("You decide to continue on into the night. Good luck!\n");
        } else {
            type ( true,
                "\nWhat would you like to do?"
                "\n1. Explore"
                "\n2. Display Stats"
                "\n3. Gather Resources"
                "\n4. Craft"
                "\n5. Rest"
                "\n6. Quit\n"
            );
            do choice = input("Enter choice: ");
            while (!choice.isChoice(true, "explore", 1, "display stats", 2, "gather resources", 3, "craft", 4, "rest", 5, "quit", 6));
            
            if (choice.isChoice("explore", 1)) {
                explore(player);
                time--;
                energy--;
            } else if (choice.isChoice("display stats", 2)) {
                player.displayStats();
                player.resources.displayResources();
            } else if (choice.isChoice("gather resources", 3)) {
                player.gatherResources();
                time--;
                energy--;
            } else if (choice.isChoice("craft", 4)) {
                player.craft();
            } else if (choice.isChoice("rest", 5)) {
                type("You find a place to rest and gain some health.\n");
                const float HEALING_MULTIPLIER = randint(10, 15) / 10.f;
                player.health += (player.maxhealth - player.health) * HEALING_MULTIPLIER / 2;
                /*
                function: f(h) = r(m - h) / 2 where
                f(h) = healing
                m = max health
                h = current health
                r = random number between 1.0 and 1.5
                */
               time--;
            } else {
                Choice quitChoice;
                do quitChoice = input("Are you sure you want to quit (1. Yes / 2. No)? ");
                while (!quitChoice.isChoice(true, "yes", 1, "no", 2));
                if (quitChoice.isChoice("yes", 1))
                    return;
            }

            levelUp(player, hitdie);
        }
    }
}

void start() {
    Player::RACE pRace;
    Player::CLASS pClass;
    uint16_t hitdie = 0;
    uint16_t str, con, def = 0;
    
    { // Get the player's choice of race
        type (
            "Hello! Welcome to the game.\nPlease select a race for your character:"
            "\n1. Elf       - Elves are a magical people with strong ties to nature. They are proficient with magical items."
            "\n2. Human     - Humans are adaptable and resilient, thriving in any environment. They are proficient with heavy weapons and armor."
            "\n3. Drakonian - Drakonians are a proud, ancient race born from the blood of dragons. They are known for their great strength and fiery breath.\n"
        );
        Choice raceChoice;
        do raceChoice = input("Please enter your choice: ");
        while (!raceChoice.isChoice(true, "elf", 1, "human", 2, "drakonian", 3));
        
        if (raceChoice.isChoice("elf", 1)) {
            pRace = Player::ELF;
            con = 2;
        } else if (raceChoice.isChoice("human", 2)) {
            pRace = Player::HUMAN;
            con = 2;
        } else {
            pRace = Player::DRAKONIAN;
            con = 4;
            def = 1;
        }
    }
    
    { // Get the player's choice of class
        type (
            "\nNow select a class:"
            "\n1. Fighter   - Trained with blades of all sorts"
            "\n2. Rogue     - A silent assassin\n"
        );
        Choice classChoice;
        do classChoice = input("Please enter your choice: ");
        while (!classChoice.isChoice(true, "fighter", 1, "rogue", 2));
        
        if (classChoice.isChoice("fighter", 1)) {
            pClass = Player::FIGHTER;
            hitdie = 10;
            str = 5;
        } else {
            pClass = Player::ROGUE;
            hitdie = 8;
            str = 4;
        }
    }

    std::string pName = inputline("\nPlease enter a name for your character: ");
    
    type("\nNice to meet you, ", pName, ". Here are your stats:\n");
    Player player(pName, pRace, pClass, hitdie + con, str, con);
    player.defense += def;
    gameLoop(player, hitdie);
}