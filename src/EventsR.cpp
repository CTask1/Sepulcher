//CTask
#include"..\include\Choice.h"
#include"..\include\EventsR.h"
#include"..\include\Events.h"
#include"..\include\Player.h"
#include"..\include\Enemy.h"
#include"..\include\util.h"

void EventsR::lostTraveler() { // Lost Traveler
    type (
        "You encounter a lost traveler who is in need of directions.\n"
        "What would you like to do?\n"
        "1. Offer help\n"
        "2. Ignore them\n"
    );
    int travelerChoice;
    do travelerChoice = Choice(input(prompt.data())).isChoice({"offer help", "ignore them"});
    while (!travelerChoice == 0);
    
    if (travelerChoice == 2) {
        type("\nYou choose to ignore the lost traveler.\n");
        return;
    }
    type("\nYou offer help to the lost traveler. ");
    int hostile = randint(1, 3); // 33% chance the traveler is hostile and attacks
    if (hostile == 1) {
        type("As you assist the traveler, they turn around and attack you!\n");
        events.initCombat(Enemy::TRAVELER, true);
    } else {
        uint16_t expGain = (randint(1, 100) == 1) ? randint(35, 75) : randint(10, 20); // 1% chance of extra exp
        type("They are grateful and share a bit of their wisdom.\nYou gained ", expGain, " experience points!\n");
        player.addExp(expGain);
    }
}

void EventsR::mountainPass() { // Mountain Pass
    type (
        "You come across a treacherous mountain pass. Climbing it will be challenging."
        "\nWhat would you like to do?"
        "\n1. Attempt the climb"
        "\n2. Find an alternate route\n"
    );
    int climbChoice;
    do climbChoice = Choice(input(prompt.data())).isChoice({"attempt the climb", "find an alternate route"});
    while (climbChoice == 0);
    
    if (climbChoice == 1) {
        type("\nYou decide to attempt the climb. It's steep and dangerous, but you press on.\n");
        int success = randint(1, 3); // 33% chance of failure
        if (success != 1)
            type("You successfully navigate the mountain pass without incident.\n");
        else {
            type("The climb proves challenging. You slip and fall, losing some health.\n");
            player.health = ui16(std::max(player.health - randint(player.maxHealth / 4, player.maxHealth / 3), 0));
        }
        return;
    }
    int altRouteEvent = randint(1, 4);
    switch (altRouteEvent) {
    case 1: {
        type("As you take the alternate route, you find yourself ambushed by a bandit!\n");
        events.initCombat(Enemy::BANDIT, true);
        break;
    } case 2:
        type (
            "You come across a group of friendly travelers along the alternate route."
            "\nThey share supplies and offer valuable information.\n"
        );
        player.receiveGift();
        break;
    case 3: {
        type (
            "The alternate route leads you to a crossroads with mysterious markings. Choose a path carefully."
            "\n1. Follow the left path"
            "\n2. Follow the right path\n"
        );
        int pathChoice;
        do pathChoice = Choice(input(prompt.data())).isChoice({"follow the left path", "follow the right path"});
        while (pathChoice == 0);
        
        if (pathChoice == 1) {
            type("\nYou decide to follow the left path, guided by the mysterious markings.\n");
            int outcome = randint(1, 2);
            if (outcome == 1) {
                type("The left path leads to a peaceful glade with a group of friendly travelers. They share supplies with you!\n");
                player.receiveGift();
            } else {
                type (
                    "As you venture down the left path, you stumble upon an abandoned campsite. Investigate or continue?"
                    "\n1. Investigate the campsite"
                    "\n2. Continue on the path\n"
                );
                int campsiteChoice;
                do campsiteChoice = Choice(input(prompt.data())).isChoice({"investigate the campsite", "continue on the path"});
                while (campsiteChoice == 0);
                
                if (campsiteChoice == 2) {
                    type("You choose to continue on the path, avoiding potential danger at the abandoned campsite.\n");
                    return;
                }
                type("\nYou decide to investigate the campsite. It seems abandoned, but you find a hidden stash of supplies!\n");
                player.receiveGift();
                if (randint(1, 4) == 1) {
                    type("\nAs you are leaving the campsite, you are ambushed by a bandit!\n");
                    events.initCombat(Enemy::BANDIT, true);
                }
            }
            return;
        }
        type("\nYou choose to follow the right path, intrigued by the mysterious markings.\n");
        int outcome = randint(1, 2);
        if (outcome == 1) {
            type("\nThe right path leads to a dense forest with unique flora.\nYou collect rare herbs with medicinal properties and regain health!\n");
            player.heal(3);
            if (player.Class == Player::WIZARD && player.mana != player.maxMana)
                player.mana++;
            return;
        }
        type (
            "\nAs you venture down the right path, you encounter a wild animal. How will you react?"
            "\n1. Try to calm the animal"
            "\n2. Retreat slowly\n"
        );
        int animalChoice;
        do animalChoice = Choice(input(prompt.data())).isChoice({"try to calm the animal", "retreat slowly"});
        while (animalChoice == 0);
        
        if (animalChoice == 1) {
            type("\nYou attempt to calm the wild animal. Surprisingly, it responds peacefully and leaves.\n");
        } else
            type("\nYou choose to retreat slowly, avoiding confrontation with the wild animal.\n");
        break;
    } case 4:
        type("You find nothing noteworthy along the alternate route.\n");
        break;
    }
}

void EventsR::mysteriousCave() { // Mysterious Cave
    type (
        "You discover a mysterious cave entrance. It emanates an eerie glow from within."
        "\nWhat would you like to do?"
        "\n1. Enter the cave"
        "\n2. Move on\n"
    );
    int caveChoice;
    do caveChoice = Choice(input(prompt.data())).isChoice({"enter the cave", "move on"});
    while (caveChoice == 0);
    
    if (caveChoice == 2) {
        type("You choose to stay cautious and continue exploring.\n");
        return;
    }
    type("You decide to enter the mysterious cave, drawn by its allure.\n");
    int encounter = randint(1, 2); // 50% chance for each event
    if (encounter == 1) {
        type("The cave is filled with valuable crystals. You gain some resources!\n");
        player.resources["Crystals"] += randint(1, player.level);
    }
    else {
        type("The cave is home to a hostile creature!\n");
        events.initCombat(Enemy::CAVE_CREATURE);
    }
}

void EventsR::strangeAmulet() { // Strange Amulet
    type (
        "You spot a glimmer beneath a pile of fallen leaves and uncover an ornate amulet.\n"
        "Its surface is engraved with swirling patterns and a faint hum resonates from within.\n"
        "Something about it draws you in, but its nature is uncertain.\n"
        "What would you like to do?\n"
        "1. Pick it up\n"
        "2. Leave it\n"
    );
    int choice;
    do choice = Choice(input(prompt.data())).isChoice({"pick it up", "leave it"});
    while (choice == 0);

    if (choice == 2) {
        type (
            "You decide taking the amulet isn't worth the risk. As you turn away, a whisper drifts through the air.\n"
            "When you glance back, the amulet is gone.\n"
        );
        return;
    }
    uint16_t amuletType = randint(1, 5);
    switch (amuletType) {
    case 1: {
        type (
            "The amulet pulses with powerful energy.\n"
            "You feel stronger, faster, and ready for anything.\n"
        );
        player.initSpecial(Item::TYPE::SPL_AM_WARBORN, Item::Source::FIND);
        player.resources.addResource("Amulet of the Warborn");
        break;
    } case 2: {
        type (
            "A soothing presence envelops you, as if unseen hands are guiding your movements.\n"
            "Your reflexes feel sharper, and your stance more solid.\n"
        );
        player.initSpecial(Item::TYPE::SPL_AM_GUARDIAN, Item::Source::FIND);
        player.resources.addResource("Amulet of the Guardian");
        break;
    } case 3: {
        type (
            "Your vision flickers, and for a moment you feel detached from reality.\n"
            "The world seems... quieter.\n"
        );
        player.initSpecial(Item::TYPE::SPL_AM_SHADOW, Item::Source::FIND);
        player.resources.addResource("Amulet of the Shadow");
        break;
    } case 4: {
        type (
            "The amulet pulses, filling you with an insatiable hunger.\n"
            "You feel stronger, but your sense of caution wanes.\n"
        );
        player.initSpecial(Item::TYPE::SPL_AM_FURY, Item::Source::FIND);
        player.resources.addResource("Amulet of Fury");
        break;
    } case 5: {
        type (
            "A sharp pain strikes your chest, and a dreadful chill spreads through your body.\n"
            "The weight of unseen sorrow presses against your heart.\n"
        );
        player.initSpecial(Item::TYPE::SPL_AM_WEEPING, Item::Source::FIND);
        player.resources.addResource("Amulet of the Weeping Spirit");
        break;
    }}
}