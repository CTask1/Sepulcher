//CTask
#pragma once
#include"Enemy.h"
#include"EventsS.h"
#include"EventsR.h"
#include"EventsD.h"

class Player;

class Events {
public:

    Events(Player& p) : player(p), eS(*this, p), eR(*this, p), eD(*this, p) {}

    Enemy::Enemy initEnemy(const Enemy::TYPE eType, const bool boss = false);
    
    bool combat(Enemy::Enemy&& enemy, bool surprised = false, const bool sepulcher = false, const char* message = nullptr);
    
    
    /* ----- SAFE EVENTS ----- */
    void oldChest()         { eS.oldChest         (); } // Old Chest
    void hiddenArmory()     { eS.hiddenArmory     (); } // Hidden Armory
    void travelingTrader()  { eS.travelingTrader  (); } // Traveling Trader
    void friendlyTraveler() { eS.friendlyTraveler (); } // Friendly Traveler
    void nothing()          { eS.nothing          (); } // Nothing
    void stream()           { eS.stream           (); } // Stream
    void garden()           { eS.garden           (); } // Hidden Garden
    void meadow()           { eS.meadow           (); } // Peaceful Meadow
    void statue()           { eS.statue           (); } // Statue

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

bool checkDeath(Player&, Enemy::Enemy&);

void explore(Player&);