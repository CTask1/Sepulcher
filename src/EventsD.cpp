//CTask
#include"..\include\Choice.h"
#include"..\include\EventsD.h"
#include"..\include\Events.h"
#include"..\include\Player.h"
#include"..\include\Enemy.h"
#include"..\include\util.h"

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
            type("The log knocks you to the ground and you take ", damage, " damage.\n");
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
            type("You are knocked to the ground by a flying rock, taking ", damage, " damage.\n");
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
            type("The thorns cut through your skin and you take ", damage, " damage.\n");
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

namespace {
    bool sepulcherContinue() {
        setOutputSettings(false, 25);
        type("\nDo you wish to continue (1. Yes / 2. No)?\n");
        Choice continueChoice;
        do continueChoice = input("Enter choice: ");
        while (!continueChoice.isChoice(true, "yes", 1, "no", 2));

        if (continueChoice.isChoice("no", 2)) {
            setOutputSettings(false, 20);
            type("\nYou decide to turn back, leaving the sepulcher to its slumber.\n");
            return 0;
        }
        return 1;
    }
}

void EventsD::sepulcher() {
    type (
        "You encounter an ancient sepulcher. A cold wind howls through its ruined archway, carrying whispers in a language long forgotten."
        "\nThe stones beneath your feet are slick with a substance thicker than water, seeping from unseen cracks."
        "\nA silent presence lingers beyond the veil of shadow, watching, waiting."
        "\nWhat would you like to do?"
        "\n1. Enter"
        "\n2. Turn back\n"
    );
    Choice enterChoice;
    do enterChoice = input("Enter choice: ");
    while (!enterChoice.isChoice(true, "enter", 1, "turn back", 2));

    if (enterChoice.isChoice("turn back", 2)) {
        type("\nYou decide to turn back, leaving the sepulcher to its slumber.\n");
        return;
    }

    type("\nYou decide to enter the sepulcher...\n");

    uint16_t message = randint(1, 2);
    setOutputSettings(false, 20);
    switch (message) {
    case 1:
        type (
            "As you walk through the empty halls, your heartbeat echoes unnaturally, as if something else is beating alongside it."
            "\nA low, guttural sound—too deep to be human—resonates through the walls."
            "\nThe air thickens with an unseen presence. Each step forward is heavier than the last.\n"
        );
        break;
    case 2:
        type (
            "Faint skeletal hands reach out from the walls as you pass, fingers twitching as if grasping for something long lost."
            "\nTorches flicker with unnatural blue flames, casting shadows that move without their source."
            "\nA voice—your own, yet not—whispers from the darkness: \"Turn back...\"\n"
        );
        break;
    }

    if (!sepulcherContinue()) return;
    setOutputSettings(false, 25);
    type("\nYou decide to venture deeper into the sepulcher, where the stench of decay grows stronger...\n");

    uint16_t encounter = randint(1, 3);
    setOutputSettings(false, 25);
    switch (encounter) {
    case 1:
        type("Suddenly, a wraith materializes before you, screeching with rage!\n");
        events.initCombat(Enemy::WRAITH);
        break;
    case 2:
        type("A skeletal warrior rises from the ground, its eyes glowing with a malevolent light!\n");
        events.initCombat(Enemy::SKELETON);
        break;
    case 3:
        type("A cursed spirit emerges from the shadows, its form shifting and writhing!\n");
        events.initCombat(Enemy::SPIRIT);
        break;
    }

    if (!sepulcherContinue()) return;
    setOutputSettings(false, 25);
    type (
        "\nYou descend into the lowest chamber. The silence is suffocating."
        "\nCarvings on the walls depict a forgotten kingdom, its people twisted in agony.\n"
    );
    wait(500);
    setOutputSettings(false, 25);
    type (
        "\nSuddenly, the sarcophagus at the center of the room bursts open."
        "\nA figure, cloaked in shadows, rises from within, its eyes burning with an otherworldly fire.\n"
    );
    events.initCombat(Enemy::KING);
    setOutputSettings(false, 25);
    type (
        "\nWith a final, echoing scream, the figure collapses into a pile of ash."
        "\nThe sepulcher begins to tremble, dust cascading from the ceiling as the very walls groan in protest.\n"
        "\nYou sprint back through the halls, the walls closing in around you."
        "\nYou emerge into the light, gasping for breath, the weight of the sepulcher's curse lifted from your shoulders."
        "\nBehind you the ancient structure crumbles, sealing its secrets away forever.\n"
        "\nBirdsong returns to the skies above, as if the world itself sighs in relief."
        "\nYou have faced the darkness and emerged victorious, a testament to your strength and resilience."
        "\nThe journey is over, but you are no longer the same adventurer who entered that cursed tomb."
        "\nYou are a warrior, a survivor, a legend.\n"
    );
    wait(1000);
    setOutputSettings(true);
    type (
        "\nThanks for playing!\n"
        "\nPress Enter key to continue . . . "
    );
    std::cin.ignore(SIZE_MAX, '\n');
    std::cin.get();
    exit(0);
}