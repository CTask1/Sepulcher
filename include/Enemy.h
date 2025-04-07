//CTask1
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
        { "Bandit"       , 0.95f, 1.05f },
        { "Bear"         , 1.10f, 1.00f },
        { "Cave Creature", 1.05f, 1.05f },
        { "Goblin"       , 0.90f, 1.05f },
        { "Poacher"      , 1.00f, 1.00f },
        { "Dark Shadow"  , 1.00f, 1.05f },
        { "Cursed Spirit", 1.05f, 1.10f },
        { "Lost Traveler", 0.95f, 1.00f }
    };

    class Enemy {
    public:
        std::string_view name;
        uint16_t health;
        uint16_t strength;

        Enemy() : name("None"), health(0), strength(0) {}

        Enemy(TYPE type, uint16_t playerHealth, uint16_t playerStrength, uint16_t playerLevel) :
            name(eType[static_cast<uint16_t>(type)].name),
            health (
                (uint16_t) (
                    randint (
                        playerHealth,
                        (uint16_t)std::round(playerHealth * eType[static_cast<uint16_t>(type)].healthMod)
                    ) * (playerLevel / 25.0 + 1)
                )
            ),
            strength (
                (uint16_t) (
                    randint (
                        playerStrength,
                        (uint16_t)std::round(playerStrength * eType[static_cast<uint16_t>(type)].strengthMod)
                    ) * (playerLevel / 25.0 + 1)
                )
            ) {}

        void displayStats() {
            setOutputSettings();
            std::cout << "\n-------------------------";
            type("\nEnemy: ", name, "\nHealth: ", health, "\nStrength: ", strength);
            std::cout << "\n-------------------------\n";
        }
    };
}