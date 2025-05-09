//CTask
#pragma once

class Player;
class Events;

class EventsD {
public:

    EventsD(Events& e, Player& p) : events(e), player(p) {}

    void enemyEncounter(); // Enemy Encounter
    void hunterTrap(); // Poacher's Snare
    void sepulcher(); // Sepulcher

private:
    Player& player;
    Events& events;
};