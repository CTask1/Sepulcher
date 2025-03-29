#pragma once
#include"Player.h"

class Events;

class EventsS {
public:

    EventsS(Events& e, Player& p) : events(e), player(p) {}

    void oldChest(); // Old Chest
    void hiddenArmory(); // Hidden Armory
    void travelingTrader(); // Traveling Trader
    void friendlyTraveler(); // Friendly Traveler

private:
    Player& player;
    Events& events;
};