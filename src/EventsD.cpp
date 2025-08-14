//CTask
#include<iostream>

#include"TextManager.h"
#include"Choice.h"
#include"EventsD.h"
#include"Events.h"
#include"Player.h"
#include"Enemy.h"
#include"util.h"

void EventsD::enemyEncounter() {
    int encounter = randint(1, 3);
    switch (encounter) {
    case 1:
        type(TM::get("events.dangerous.encounter.shadow"));
        events.combat(events.initEnemy(Enemy::SHADOW));
        break;
    case 2:
        type(TM::get("events.dangerous.encounter.goblin"));
        events.combat(events.initEnemy(Enemy::GOBLIN));
        break;
    case 3:
        type(TM::get("events.dangerous.encounter.spirit"));
        events.combat(events.initEnemy(Enemy::SPIRIT));
        break;
    }
}

void EventsD::hunterTrap() {
    if (player.arcaneEye) {
        type(TM::get("events.dangerous.trap.arcane_eye"));
        return;
    }
    int trapType = randint(1, 7);
    bool success = (randint(1, 3) != 1);
    uint16_t damage = randint(1, player.health / 3);
    switch (trapType) {
    case 1:
        type(TM::get("events.dangerous.trap.rope_snare.message"));
        wheel(3);
        type(TM::getForCondition("events.dangerous.trap.rope_snare.escape", success));
        events.combat(events.initEnemy(Enemy::POACHER), !success);
        break;
    case 2:
        type(TM::get("events.dangerous.trap.falling_net.message"));
        wheel(3);
        type(TM::getForCondition("events.dangerous.trap.falling_net.escape", success));
        events.combat(events.initEnemy(Enemy::POACHER), !success);
        break;
    case 3:
        type(TM::get("events.dangerous.trap.swinging_log.message"));
        wheel(3);
        type(TM::getForCondition("events.dangerous.trap.swinging_log.escape", success, {
            .replacements = {{"{damage}", std::to_string(damage)}}
        }));
        if (!success)
            player.health -= damage;
        break;
    case 4:
        type(TM::get("events.dangerous.trap.rock_avalanche.message"));
        wheel(3);
        type(TM::getForCondition("events.dangerous.trap.rock_avalanche.escape", success, {
            .replacements = {{"{damage}", std::to_string(damage)}}
        }));
        if (!success)
            player.health -= damage;
        break;
    case 5:
        type(TM::get("events.dangerous.trap.magical_vines.message"));
        wheel(3);
        type(TM::getForCondition("events.dangerous.trap.magical_vines.escape", success, {
            .replacements = {{"{damage}", std::to_string(damage)}}
        }));
        if (!success)
            player.health -= damage;
        break;
    case 6:
        type(TM::get("events.dangerous.trap.hidden_assassin.message"));
        events.combat(events.initEnemy(Enemy::POACHER));
        break;
    case 7:
        type(TM::get("events.dangerous.trap.carnivore_den.message"));
        events.combat(events.initEnemy(Enemy::BEAR));
        break;
    }
}

bool sepulcherContinue() {
    setDelay(25);
    type(TM::get("events.dangerous.sepulcher.continue_check"));
    int continueChoice;
    do continueChoice = Choice(input(prompt.data())).isChoice({"yes", "no"});
    while (continueChoice == 0);

    if (continueChoice == 2) {
        setDelay(20);
        type(TM::get("events.dangerous.sepulcher.leave"));
        return 0;
    }
    return 1;
}

void EventsD::sepulcher() {
    type (
        TM::get("events.dangerous.sepulcher.intro.message")       +
        TM::get("events.dangerous.sepulcher.intro.choose_option") +
        TM::getAllAsStr("events.dangerous.sepulcher.intro.options")
    );

    if (Choice::getChoice(TM::getAllAsLst("events.dangerous.sepulcher.intro.options")) == 2) {
        type(TM::get("events.dangerous.sepulcher.leave"));
        return;
    }
    type(TM::get("events.dangerous.sepulcher.enter"));

    setDelay(20);
    type(TM::getForNumber("events.dangerous.sepulcher.stage_1.messages", randint(1, 2), {.end = '\n'}));

    if (!sepulcherContinue()) return;

    setDelay(25);
    type(TM::get("events.dangerous.sepulcher.stage_1.continue"));

    uint16_t encounter = randint(1, 3);
    const char* encounterMessage = TM::getForNumber("events.dangerous.sepulcher.stage_2.encounters", randint(1, 3)).c_str();
    setDelay(15);
    switch (encounter) {
    case 1:
        if (!events.combat(events.initEnemy(Enemy::WRAITH), false, true, encounterMessage))
            return;
        break;
    case 2:
        if (!events.combat(events.initEnemy(Enemy::SKELETON), false, true, encounterMessage))
            return;
        break;
    case 3:
        if (!events.combat(events.initEnemy(Enemy::SPIRIT), false, true, encounterMessage))
            return;
        break;
    }

    if (!sepulcherContinue()) return;

    setDelay(25);
    type(TM::get("events.dangerous.sepulcher.stage_2.continue"));
    
    if (!sepulcherContinue()) return;

    setDelay(25);
    type(TM::get("events.dangerous.sepulcher.stage_3.intro"));
    wait(500);
    setDelay(15);
    type(TM::get("events.dangerous.sepulcher.stage_3.king_encounter_1"));
    if (!events.combat(events.initEnemy(Enemy::KING, true), false, true))
        return;
    setDelay(25);
    type(TM::get("events.dangerous.sepulcher.stage_3.king_death_1"));
    wait(500);
    setDelay(500);
    type(TM::get("events.dangerous.sepulcher.stage_3.interlude"));
    wait(400);
    setDelay(20);
    type(TM::get("events.dangerous.sepulcher.stage_3.king_revive"));
    wait(500);
    setDelay(50);
    type(TM::get("events.dangerous.sepulcher.stage_3.king_realization_1"));
    wait(200);
    setDelay(50);
    type(TM::get("events.dangerous.sepulcher.stage_3.king_realization_2"));
    setDelay(30);
    type(TM::get("events.dangerous.sepulcher.stage_3.king_realization_3"));
    setDelay(30);
    if (!events.combat(events.initEnemy(Enemy::ENRAGED_KING, true), false, true,
    TM::get("events.dangerous.sepulcher.stage_3.king_encounter_2", {.end = '\0'}).c_str()))
        return;
    setDelay(25);
    type (
        TM::get("events.dangerous.sepulcher.stage_3.king_death_2") +
        TM::get("events.dangerous.sepulcher.escape")       +
        TM::get("events.dangerous.sepulcher.victory")
    );
    wait(800);
    setDelay(30);
    type(TM::get("events.dangerous.sepulcher.mystery"));
    wait(1000);
    setMode(LIST_OUT);
    type(TM::get("events.dangerous.sepulcher.end"));
    std::cin.ignore(SIZE_MAX, '\n');
    std::cin.get();
    exit(0);
}