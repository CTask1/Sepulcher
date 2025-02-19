#pragma once
#include"Player.h"
#include"Enemy.h"

class Events;

class EventsR {
public:

    EventsR(Events& e, Player& p) : events(e), player(p) {}

    void mountainPass(); // Mountain Pass
    void mysteriousCave(); // Mysterious Cave
    void strangeAmulet(); // Strange Amulet
    void lostTraveler(); // Lost Traveler

private:
    Player& player;
    Events& events;
};