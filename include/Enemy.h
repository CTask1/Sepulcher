#pragma once
#include<unordered_map>
#include<string>
#include"Util.h"

namespace Enemy {
    enum TYPE {
        BANDIT,
        BEAR,
        CAVE_CREATURE,
        GOBLIN,
        POACHER,
        SHADOW,
        SPIRIT,
        TRAVELER
    };

    struct Info {
        std::string_view name;
        float healthMod;
        float attackMod;
    };

    constexpr Info eType[] {
        { "Bandit"       , 1.00f, 1.25f },
        { "Bear"         , 1.20f, 1.40f },
        { "Cave Creature", 1.20f, 1.25f },
        { "Goblin"       , 0.80f, 1.10f },
        { "Poacher"      , 1.00f, 1.20f },
        { "Dark Shadow"  , 1.00f, 1.40f },
        { "Cursed Spirit", 1.15f, 1.35f },
        { "Lost Traveler", 1.10f, 1.30f }
    };

    class Enemy {
    public:
        std::string_view name;
        uint16_t health;
        uint16_t attack;

        Enemy() : name("None"), health(0), attack(0) {}

        Enemy(TYPE t, uint16_t h, uint16_t a, uint16_t l) :
            name(eType[static_cast<uint16_t>(t)].name),
            health(std::round(h * eType[static_cast<uint16_t>(t)].healthMod)),
            attack(a + std::round(pow(l, eType[static_cast<uint16_t>(t)].attackMod))) {}

        void displayStats() {
            setOutputSettings();
            std::cout << "\n-------------------------";
            type("\nEnemy: ", name, "\nHealth: ", health, "\nAttack: ", attack);
            std::cout << "\n-------------------------\n";
        }
    };
}