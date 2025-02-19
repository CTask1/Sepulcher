#pragma once
#include"Player.h"
#include"Enemy.h"

class Events;

class EventsD {
public:

    EventsD(Events& e, Player& p) : events(e), player(p) {}

    void enemyEncounter(); // Enemy Encounter
    void hunterTrap(); // Poacher's Snare

private:
    Player& player;
    Events& events;
};