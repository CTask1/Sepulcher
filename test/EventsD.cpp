#include"EventsD.h"
#include"Events.h"

void EventsD::enemyEncounter() { // Enemy Encounter
    int encounter = randint(1, 3);
    switch (encounter) {
    case 1: {
        type("You stumble upon a dark cave. As you enter, you encounter a fearsome enemy!\n");
        events.initCombat(Enemy::SHADOW);
        break;
    } case 2: {
        type("You find a lit campfire. As you approach, you encounter a fearsome enemy!\n");
        events.initCombat(Enemy::GOBLIN);
        break;
    } case 3: {
        type("You venture into a cursed forest. Strange occurrences unsettle your senses.\n");
        events.initCombat(Enemy::SPIRIT);
        break;
    }}
}

void EventsD::hunterTrap() { // Poacher's Snare
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
        events.initCombat(Enemy::POACHER, fail);
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
        events.initCombat(Enemy::POACHER, fail);
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
        events.initCombat(Enemy::POACHER);
        break;
    } case 7: { // Carnivore's Den
        type (
            "You stumble into a deep pit, landing on something hard...bones; dozens of them."
            "\nA low growl rumbles behind you as a pair of glowing eyes emerge from the darkness."
            "\nWhatever killed the owners of these bones is not done feeding."
        );
        events.initCombat(Enemy::BEAR);
        break;
    }}
}