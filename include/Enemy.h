//CTask
#pragma once
#include<string_view>
#include<iostream>
#include<cmath>

#include"util.h"

namespace Enemy {
    enum TYPE {
        // General
        BANDIT,
        CAVE_CREATURE,
        GOBLIN,
        SHADOW,
        SPIRIT,
        TRAVELER,
        // Poacher's Snare
        BEAR,
        POACHER,
        // Sepulcher
        SKELETON,
        WRAITH,
        KING
    };

    struct Info {
        std::string_view name;
        float healthMod;
        float strengthMod;
    };

    constexpr Info eType[] {
        // Name ---------- hMod - strMod -
        { "Bandit"       , 0.95f, 1.05f },
        { "Cave Creature", 1.05f, 1.05f },
        { "Goblin"       , 0.90f, 1.05f },
        { "Dark Shadow"  , 1.00f, 1.05f },
        { "Cursed Spirit", 1.05f, 1.10f },
        { "Lost Traveler", 0.95f, 1.00f },
        { "Bear"         , 1.10f, 1.00f },
        { "Poacher"      , 1.00f, 1.00f },
        { "Skeleton"     , 0.90f, 1.10f },
        { "Wraith"       , 1.05f, 1.10f },
        { "Revenant King", 1.25f, 1.10f }
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
                    randint ( // random number between playerHealth and playerHealth * healthMod
                        playerHealth,
                        (uint16_t)std::round(playerHealth * eType[static_cast<uint16_t>(type)].healthMod)
                    ) * (playerLevel / 25.0 + 1) // add 4% per level
                )
            ),
            strength (
                (uint16_t) (
                    randint ( // random number between playerStrength and playerStrength * strengthMod
                        playerStrength,
                        (uint16_t)std::round(pow(playerStrength, 1.15f) * eType[static_cast<uint16_t>(type)].strengthMod)
                    ) * (playerLevel / 25.0 + 1) // add 4% per level
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