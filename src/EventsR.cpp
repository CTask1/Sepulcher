//CTask
#include"TextManager.h"
#include"Choice.h"
#include"EventsR.h"
#include"Events.h"
#include"Player.h"
#include"Enemy.h"
#include"util.h"

void EventsR::lostTraveler() {
    type (
        TM::get("events.risky.lost_traveler.message")       +
        TM::get("events.risky.lost_traveler.choose_option") +
        TM::getAllAsStr("events.risky.lost_traveler.options")
    );
    
    if (Choice::getChoice(TM::getAllAsLst("events.risky.lost_traveler.options")) == 2) {
        type(TM::get("events.risky.lost_traveler.ignore"));
        return;
    }
    type(TM::get("events.risky.lost_traveler.help"));
    int hostile = randint(1, 3); // 33% chance the traveler is hostile and attacks
    if (hostile == 1) {
        type(TM::get("events.risky.lost_traveler.outcomes.attack"));
        events.combat(events.initEnemy(Enemy::TRAVELER), true);
    } else {
        uint16_t expGain = (randint(1, 100) == 1) ? randint(35, 75) : randint(10, 20); // 1% chance of extra exp
        type(TM::get("events.risky.lost_traveler.outcomes.thank", {
            .replacements = {{"{exp}", std::to_string(expGain)}}
        }));
        player.addExp(expGain);
    }
}

void EventsR::mountainPass() {
    type (
        TM::get("events.risky.mountain_pass.message")       +
        TM::get("events.risky.mountain_pass.choose_option") +
        TM::getAllAsStr("events.risky.mountain_pass.options")
    );
    
    if (Choice::getChoice(TM::getAllAsLst("events.risky.mountain_pass.options")) == 1) {
        type(TM::get("events.risky.mountain_pass.attempt.message"));
        int success = randint(1, 3); // 33% chance of failure
        type(TM::getForCondition("events.risky.mountain_pass.attempt.success", success));
        if (success == 1)
            player.health = ui16(std::max(player.health - randint(player.maxHealth / 4, player.maxHealth / 3), 0));
        return;
    }
    int altRouteEvent = randint((day ? 1 : (randint(1, 3) == 1 ? 1 : 2)), 4);
    switch (altRouteEvent) {
    case 1:
        type(TM::get("events.risky.mountain_pass.alternate.travelers"));
        player.receiveGift();
        break;
    case 2:
        type(TM::get("events.risky.mountain_pass.alternate.ambush"));
        events.combat(events.initEnemy(Enemy::BANDIT), true);
        break;
    case 3: {
        type (
            TM::get("events.risky.mountain_pass.alternate.crossroads.choose_option") +
            TM::getAllAsStr("events.risky.mountain_pass.alternate.crossroads.options")
        );
        
        if (Choice::getChoice(TM::getAllAsLst("events.risky.mountain_pass.alternate.crossroads.options")) == 1) {
            type(TM::get("events.risky.mountain_pass.alternate.crossroads.left.message"));
            int outcome = randint((day ? 1 : randint(1, 3)), 2);
            if (outcome == 1) {
                type(TM::get("events.risky.mountain_pass.alternate.crossroads.left.travelers"));
                player.receiveGift();
                return;
            }
            type (
                TM::get("events.risky.mountain_pass.alternate.crossroads.left.campsite.choose_option") +
                TM::getAllAsStr("events.risky.mountain_pass.alternate.crossroads.left.campsite.options")
            );
            
            if (Choice::getChoice(TM::getAllAsLst("events.risky.mountain_pass.alternate.crossroads.left.campsite.options")) == 2) {
                type(TM::get("events.risky.mountain_pass.alternate.crossroads.left.campsite.continue"));
                return;
            }
            type(TM::get("events.risky.mountain_pass.alternate.crossroads.left.campsite.investigate"));
            player.receiveGift();
            if (randint(1, 3) == 1) {
                type(TM::get("events.risky.mountain_pass.alternate.crossroads.left.campsite.ambush"));
                events.combat(events.initEnemy(Enemy::BANDIT), true);
            }
            return;
        }
        type(TM::get("events.risky.mountain_pass.alternate.crossroads.right.message"));
        int outcome = randint((day ? 1 : randint(1, 3)), 2); // Forest is rarer at night
        if (outcome == 1) {
            type(TM::get("events.risky.mountain_pass.alternate.crossroads.right.forest"));
            player.heal(3);
            player.resources.addResource("Medicinal Herbs", randint(3, 6));
            if (player.Class == Player::CLASS::WIZARD && player.mana != player.maxMana)
                player.mana++;
            return;
        }
        type (
            TM::get("events.risky.mountain_pass.alternate.crossroads.right.animal.choose_option") +
            TM::getAllAsStr("events.risky.mountain_pass.alternate.crossroads.right.animal.options")
        );
        
        type(TM::getForNumber (
            "events.risky.mountain_pass.alternate.crossroads.right.animal.outcomes",
            Choice::getChoice(TM::getAllAsLst("events.risky.mountain_pass.alternate.crossroads.right.animal.options")),
            {.end = '\n'}
        ));
        break;
    } case 4:
        type(TM::get("events.risky.mountain_pass.alternate.nothing"));
        break;
    }
}

void EventsR::mysteriousCave() {
    type (
        TM::get("events.risky.cave.message")       +
        TM::get("events.risky.cave.choose_option") +
        TM::getAllAsStr("events.risky.cave.options")
    );
    
    if (Choice::getChoice(TM::getAllAsLst("events.risky.cave.options")) == 2) {
        type(TM::get("events.risky.cave.continue"));
        return;
    }
    type(TM::get("events.risky.cave.enter"));
    uint16_t encounter = randint(1, 2); // 50% chance for each event
    type(TM::getForNumber("events.risky.cave.outcomes", encounter));
    if (encounter == 1)
        player.resources["Crystals"] += randint(1, player.level);
    else
        events.combat(events.initEnemy(Enemy::CAVE_CREATURE));
}

void EventsR::strangeAmulet() {
    type (
        TM::get("events.risky.amulet.message")       +      
        TM::get("events.risky.amulet.choose_option") +
        TM::getAllAsStr("events.risky.amulet.options")
    );

    if (Choice::getChoice(TM::getAllAsLst("events.risky.amulet.options")) == 2) {
        type(TM::get("events.risky.amulet.leave"));
        return;
    }
    uint16_t amuletType = randint(1, 5);
    type(TM::getForNumber("events.risky.amulet.get", amuletType, {.end = '\n'}));
    switch (amuletType) {
    case 1:
        player.initSpecial(Item::TYPE::SPL_AM_WARBORN, Item::Source::FIND);
        player.resources.addResource("Amulet of the Warborn");
        break;
    case 2:
        player.initSpecial(Item::TYPE::SPL_AM_GUARDIAN, Item::Source::FIND);
        player.resources.addResource("Amulet of the Guardian");
        break;
    case 3:
        player.initSpecial(Item::TYPE::SPL_AM_SHADOW, Item::Source::FIND);
        player.resources.addResource("Amulet of the Shadow");
        break;
    case 4:
        player.initSpecial(Item::TYPE::SPL_AM_FURY, Item::Source::FIND);
        player.resources.addResource("Amulet of Fury");
        break;
    case 5:
        player.initSpecial(Item::TYPE::SPL_AM_WEEPING, Item::Source::FIND);
        player.resources.addResource("Amulet of the Weeping Spirit");
        break;
    }
}