//CTask
#pragma once
#include<vector>

#include"TextManager.h"
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
    type(TM::get("game.rest"));
    player.heal();
    if (player.Class == Player::CLASS::WIZARD && player.mana != player.maxMana)
         player.mana++;
    energy++;
}

bool useHealthPotion(Player& player) {
    if (player.resources["Health Potion"] < 1) {
        type(TM::get("player.health_potion.none"));
        return false;
    }
    player.resources["Health Potion"]--;
    player.heal(1);
    type(TM::get("player.health_potion.use", {.replacements = {{"{health}", std::to_string(player.health)}}}));
    return true;
}

void otherOptions(Player& player) {
    while (true) {
        type(TM::get("game.other.choose_action"));
        setMode(LIST_OUT);
        type(TM::getAllAsStr("game.other.actions", {
            .replacements = {{"{potions}", std::to_string(player.resources["Health Potion"])}}
        }));

        std::vector<std::string> options = TM::getAllAsLst("game.other.actions");
        if (player.Class != Player::CLASS::WIZARD)
            options.erase(options.begin() + 4);

        switch (Choice::getChoice(options)) {
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
    return (Choice::getChoice({"yes", "no"}, TM::get("confirmation", {
        .replacements = {{"{action}", "quit"}},
        .end = ' '
    }).c_str()) == 1);
}

int getAction() {
    type(TM::get("game.main.choose_action"));
    setMode(LIST_OUT);
    type(TM::getAllAsStr("game.main.actions"));
    return Choice::getChoice(TM::getAllAsLst("game.main.actions"));
}

void gameLoop(Player& player, uint16_t hitdie) {
    uint16_t time = DAY_LENGTH;
    uint16_t energy = DAY_LENGTH;
    // Main game loop
    while (true) {
        if (day && energy == 0 || !day && energy == 1)
            type(TM::get("game.exhaustion.warning"));
        if (!day) {
            if (energy == 0) {
                setDelay(20);
                type(TM::get("game.exhaustion.collapse"));
                player.addDebuff(Debuff::EXHAUSTED);
                resetStats(player, time, energy);
                day = true;
                setDelay(20);
                type (TM::get("game.exhaustion.awake"));
            }
        }
        if (time == 0) [[unlikely]] { // if the day/night is over
            if (day) {
                type(TM::get("game.night.begin"));
                type(TM::get("game.night.choose_action"));
                type(TM::getAllAsStr("game.night.actions", {
                    .replacements = {
                        {"{recommended}", (player.level < 5 || energy < 4) ? " (not recommended)" : ""}
                    }
                }));
                
                switch (Choice::getChoice(TM::getAllAsLst("game.night.actions"))) {
                case 1: {
                    const std::string ROOT = "game.night.choice.sleep.checks.";
                    type(TM::get("game.night.choice.sleep.message", {
                        .replacements = {
                            {"{rev_check}", TM::getForCondition(ROOT + "rev", player.Race == Player::RACE::REVENANT)},
                            {"{ability_check}", TM::getForCondition(ROOT + "ability", player.hasAbility)},
                            {"{wiz_check}", TM::getForCondition(ROOT + "wiz", player.Class == Player::CLASS::WIZARD)}
                        }
                    }));
                    if (player.Race == Player::RACE::REVENANT) {
                        if (player.bloodMeter < 3) {
                            type(TM::get("game.night.choice.sleep.blood_meter_not_full"));
                            player.addDebuff(Debuff::RAVENOUS);
                        }
                        player.bloodMeter = 0;
                        type(TM::get("game.night.choice.sleep.blood_meter_emptied"));
                    } else
                        player.healMax();
                    
                    player.removeDebuff(Debuff::EXHAUSTED);
                    resetStats(player, time, energy);
                    player.mana = std::min(player.maxMana, (uint16_t)(player.mana + 5));
                    break;
                } case 2:
                    type(TM::get("game.night.choice.continue"));
                    day = false;
                    time = NIGHT_LENGTH;
                    break;
                }
            } else {
                type (TM::get("game.night.end"));
                resetStats(player, time, energy);
                day = true;
            }
            
        } else {
            switch (getAction()) {
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
                type(TM::get("game.death"));
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
        type(TM::get("intro.welcome"));
        type(TM::get("intro.choose_race"));
        type(TM::getAllAsStr("intro.races"));

        switch (Choice::getChoice({"elf", "human", "drakonian", "revenant"})) {
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
        type(TM::get("intro.choose_class"));
        type(TM::getAllAsStr("intro.classes"));
        
        switch (Choice::getChoice({"fighter", "rogue", "wizard"})) {
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

    std::string pName = input(('\n' + TM::get("intro.prompt_name", {.end = ' '})).c_str());
    
    type(TM::get("intro.greet_name", {.replacements = {{"{name}", pName}}}));
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