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
    
    void combat(Enemy::Enemy&, bool);
    
    
    /* ----- SAFE EVENTS ----- */
    void oldChest()         { eS.oldChest         (); } // Old Chest
    void hiddenArmory()     { eS.hiddenArmory     (); } // Hidden Armory
    void travelingTrader()  { eS.travelingTrader  (); } // Traveling Trader
    void friendlyTraveler() { eS.friendlyTraveler (); } // Friendly Traveler
    
    /* ----- RISKY EVENTS ----- */
    void mountainPass()   { eR.mountainPass   (); } // Mountain Pass
    void mysteriousCave() { eR.mysteriousCave (); } // Mysterious Cave
    void strangeAmulet()  { eR.strangeAmulet  (); } // Strange Amulet
    void lostTraveler()   { eR.lostTraveler   (); } // Lost Traveler
    
    /* ----- DANGEROUS EVENTS ----- */
    void enemyEncounter() { eD.enemyEncounter (); } // Enemy Encounter
    void hunterTrap()     { eD.hunterTrap     (); } // Poacher's Snare
    void sepulcher()      { eD.sepulcher      (); } // Sepulcher

private:
    Player& player;
    EventsS eS;
    EventsR eR;
    EventsD eD;
};

bool checkDeath(Enemy::Enemy&, Player&);
    
void levelUp(Player&, uint16_t);

void explore(Player&);