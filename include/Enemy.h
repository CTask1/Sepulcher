//CTask
#pragma once
#include<string_view>
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
        KING,
        ENRAGED_KING
    };

    struct Info {
        std::string_view name;
        float healthMod;
        float strengthMod;
    };

    constexpr Info eType[] {
        // Name ------------------ hMod - strMod -
        { "Bandit"               , 0.95f, 1.05f },
        { "Cave Creature"        , 1.05f, 1.05f },
        { "Goblin"               , 0.90f, 1.05f },
        { "Dark Shadow"          , 1.00f, 1.05f },
        { "Cursed Spirit"        , 1.05f, 1.10f },
        { "Lost Traveler"        , 0.95f, 1.00f },
        { "Bear"                 , 1.10f, 1.00f },
        { "Poacher"              , 1.00f, 1.00f },
        { "Skeleton"             , 0.90f, 1.10f },
        { "Wraith"               , 1.05f, 1.10f },
        { "Revenant King"        , 1.40f, 1.15f },
        { "Enraged Revenant King", 1.00f, 1.80f }
    };

    class Enemy {
    public:
        std::string_view name;
        uint16_t health;
        uint16_t strength;

        Enemy() : name("None"), health(0), strength(0) {}

        Enemy(const TYPE type, const uint16_t playerHealth, const uint16_t playerStrength, const uint16_t playerLevel, const bool boss = false) :
            name(eType[ui16(type)].name),
            health (
                ui16 (
                    randint ( // random number between playerHealth and playerHealth * healthMod
                        ui16(playerHealth * (boss ? 1.1f : 1.f)), // 10% more if the enemy is a boss
                        ui16(std::round(playerHealth * eType[ui16(type)].healthMod))
                    ) * (playerLevel / 25.0 + 1) // add 4% per level
                )
            ),
            strength (
                ui16 (
                    randint ( // random number between playerStrength and playerStrength * strengthMod
                        ui16(playerStrength * (boss ? 1.75f : 1.f)), // 75% more if the enemy is a boss
                        ui16(std::round(pow(playerStrength, 1.15f) * eType[ui16(type)].strengthMod))
                    ) * (playerLevel / 25.0 + 1) // add 4% per level
                )
            ) {}

        void displayStats() {
            setOutputSettings();
            std::cout << "\n-------------------------";
            type (
                "\nEnemy: ", name,
                "\nHealth: ", health,
                "\nStrength: ", strength
            );
            std::cout << "\n-------------------------\n";
        }

        bool operator==(TYPE enemy) const {
            return name == eType[enemy].name;
        }

    };

}