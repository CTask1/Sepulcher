#pragma once
#include"Player.h"
#include"Enemy.h"

#include"EventsS.h"
#include"EventsR.h"
#include"EventsD.h"

class Events {
public:

    Events(Player& p) : player(p), eS(*this, p), eR(*this, p), eD(*this, p) {}

    void initCombat(const Enemy::TYPE, const bool = false);
    
    void combat(Enemy::Enemy&, const bool);
    
    
    /* ----- SAFE EVENTS ----- */
    void oldChest() { // Old Chest
        eS.oldChest();
    }
    
    void hiddenArmory() { // Hidden Armory
        eS.hiddenArmory();
    }
    
    void travelingTrader() { // Traveling Trader
        eS.travelingTrader();
    }
    
    void friendlyTraveler() { // Friendly Traveler
        eS.friendlyTraveler();
    }
    
    
    /* ----- RISKY EVENTS ----- */
    void mountainPass() { // Mountain Pass
        eR.mountainPass();
    }
    
    void mysteriousCave() { // Mysterious Cave
        eR.mysteriousCave();
    }
    
    void strangeAmulet() { // Strange Amulet
        eR.strangeAmulet();
    }
    
    void lostTraveler() { // Lost Traveler
        eR.lostTraveler();
    }
    
    
    /* ----- DANGEROUS EVENTS ----- */
    void enemyEncounter() { // Enemy Encounter
        eD.enemyEncounter();
    }
    
    void hunterTrap() { // Poacher's Snare
        eD.enemyEncounter();
    }

private:
    Player& player;
    EventsS eS;
    EventsR eR;
    EventsD eD;
};

void explore(Player&);