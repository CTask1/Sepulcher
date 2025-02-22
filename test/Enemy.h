#pragma once
#include<unordered_map>
#include<string>
#include"Util.h"
//import Util;

namespace Enemy {
    enum TYPE {
        BANDIT,
        BEAR,
        CAVE_CREATURE,
        GOBLIN,
        POACHER,
        SHADOW,
        SPIRIT,
        TRAVELER,
        COUNT
    };

    struct Info {
        std::string name;
        float healthMod;
        float attackMod;
    };

    inline static const Info eType[COUNT] = {
        { "Bandit", 1.f, 1.25f },
        { "Bear", 1.2f, 1.4f },
        { "Cave Creature", 1.2f, 1.25f },
        { "Goblin", 0.8f, 1.1f },
        { "Poacher", 1.f, 1.2f },
        { "Dark Shadow", 1.f, 1.4f },
        { "Cursed Spirit", 1.2f, 1.4f },
        { "Lost Traveler", 1.1f, 1.4f }
    };

    class Enemy {
    public:
        std::string name;
        uint16_t health;
        uint16_t attack;

        Enemy(TYPE t, uint16_t h, uint16_t a, uint16_t l) :
            name(eType[t].name), health(std::round(h * eType[t].healthMod)), attack(a + std::round(pow(l, eType[t].attackMod))) {}

        void displayStats() {
            std::cout << "\n-------------------------";
            type("\nEnemy: ", name, "\nHealth: ", health, "\nAttack: ", attack);
            std::cout << "\n-------------------------\n";
        }
    };
}