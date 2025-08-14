//CTask
#pragma once

class Player;
class Events;

class EventsS {
public:

    EventsS(Events& e, Player& p) : events(e), player(p) {}

    void oldChest(); // Old Chest
    void hiddenArmory(); // Hidden Armory
    void travelingTrader(); // Traveling Trader
    void friendlyTraveler(); // Friendly Traveler
    void nothing(); // Nothing
    void stream(); // Stream
    void garden(); // Hidden Garden
    void meadow(); // Peaceful Meadow
    void statue(); // Statue

private:
    Player& player;
    Events& events;
};