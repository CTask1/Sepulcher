#pragma once
#include<string_view>
#include<iostream>
#include<cmath>

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
        float strengthMod;
    };

    constexpr Info eType[] {
        { "Bandit"       , 0.90f, 1.10f },
        { "Bear"         , 1.10f, 1.00f },
        { "Cave Creature", 1.00f, 1.15f },
        { "Goblin"       , 0.80f, 1.10f },
        { "Poacher"      , 1.00f, 1.00f },
        { "Dark Shadow"  , 1.00f, 1.10f },
        { "Cursed Spirit", 1.05f, 1.20f },
        { "Lost Traveler", 1.00f, 1.00f }
    };

    class Enemy {
    public:
        std::string_view name;
        uint16_t health;
        uint16_t strength;

        Enemy() : name("None"), health(0), strength(0) {}

        Enemy(TYPE t, uint16_t h, uint16_t s, uint16_t l) :
            name(eType[static_cast<uint16_t>(t)].name),
            health((uint16_t)std::pow(randint(h, (uint16_t)std::round(h * eType[static_cast<uint16_t>(t)].healthMod)), l / 50.0 + 1)),
            strength((uint16_t)std::pow(randint(s, (uint16_t)std::round(s * eType[static_cast<uint16_t>(t)].strengthMod)), l / 50.0 + 1)) {}

        void displayStats() {
            setOutputSettings();
            std::cout << "\n-------------------------";
            type("\nEnemy: ", name, "\nHealth: ", health, "\nStrength: ", strength);
            std::cout << "\n-------------------------\n";
        }
    };
}