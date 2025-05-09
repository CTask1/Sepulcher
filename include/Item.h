//CTask
#pragma once
#include<string_view>

namespace Item {

    enum class Source {
        NONE,
        FIND,
        CRAFT,
        DROP
    };

    enum class ItemClass {
        GEN, // General
        ARM, // Armor
        WPN, // Weapon
        SPL  // Special
    };

    enum class TYPE {
        NONE,
        POTION,
        FOCUS,
        ARM_LEATHER,
        ARM_DRAKONIAN,
        ARM_IRON,
        ARM_STEEL,
        WPN_LONG,
        WPN_MAGIC,
        WPN_GREAT,
        WPN_CROSSBOW,
        WPN_ST_WARBORN,
        WPN_ST_GUARDIAN,
        WPN_ST_SHADOW,
        WPN_ST_FURY,
        WPN_ST_WEEPING,
        SPL_AM_WARBORN,
        SPL_AM_GUARDIAN,
        SPL_AM_SHADOW,
        SPL_AM_FURY,
        SPL_AM_WEEPING
    };

    const struct Info {
        ItemClass type;
        std::string_view name;
        float defMod = 0.f;
        float strMod = 0.f;
        bool canHavePrefix = true;
        bool nDef = false;
        bool nStr = false;
    };

    constexpr Info Data[] {
        { ItemClass::GEN, "None"                                                          },
        { ItemClass::GEN, "Health Potion"                                                 },
        { ItemClass::GEN, "Arcane Focus"                                                  },

        { ItemClass::ARM, "Leather Armor"               , 1.15f                           },
        { ItemClass::ARM, "Drakonian Armor"             , 1.15f                           },
        { ItemClass::ARM, "Iron Armor"                  , 1.20f                           },
        { ItemClass::ARM, "Steel Armor"                 , 1.30f                           },

        { ItemClass::WPN, "Longsword"                   , 0.00f, 1.25f                    },
        { ItemClass::WPN, "Magic Sword"                 , 0.00f, 1.25f                    },
        { ItemClass::WPN, "Greatsword"                  , 0.00f, 1.25f                    },
        { ItemClass::WPN, "Crossbow"                    , 0.00f, 1.25f                    },
        { ItemClass::WPN, "Staff of the Warborn"        , 0.00f, 1.25f, false             },
        { ItemClass::WPN, "Staff of the Guardian"       , 0.00f, 1.25f, false             },
        { ItemClass::WPN, "Staff of the Shadow"         , 0.00f, 1.25f, false             },
        { ItemClass::WPN, "Staff of Fury"               , 0.00f, 1.35f, false             },
        { ItemClass::WPN, "Staff of the Weeping Spirit" , 0.00f, 1.10f, false             },

        { ItemClass::SPL, "Amulet of the Warborn"       , 1.20f, 1.20f                    },
        { ItemClass::SPL, "Amulet of the Guardian"      , 1.30f, 0.00f                    },
        { ItemClass::SPL, "Amulet of the Shadow"        , 0.00f, 0.00f                    },
        { ItemClass::SPL, "Amulet of Fury"              , 1.25f, 1.25f, false, true       },
        { ItemClass::SPL, "Amulet of the Weeping Spirit", 1.20f, 1.20f, false, true, true }
    };

    class Item {
    public:
        std::string name;
        TYPE itemType;

        Item(const TYPE type = TYPE::NONE);
        virtual ~Item();
        virtual void displayInfo(const Source source = Source::NONE) const;
        bool operator!=(const TYPE Type) const;
        bool operator==(const TYPE Type) const;

    };

    class General : public Item {
    public:

        General(const TYPE t = TYPE::NONE);
    
        void displayInfo(const Source source = Source::NONE) const override;

    };
    
    class Leveled : public Item {
    public:
        uint16_t exp;
        uint16_t level;
        uint16_t nextLevel;

        Leveled(const TYPE t = TYPE::NONE);
        virtual uint16_t levelUp();
    
    };
    
    class Armor : public Leveled {
    public:
        short defenseBonus;

        enum class Prefix {
            NONE      = 50,
            RUSTED    = 9,  // -20% defense
            WORN      = 16, // -10% defense
            HEAVY     = 15, // +10% defense
            ENCHANTED = 10  // +20% defense
        };

        enum class Suffix {
            NONE       = 75,
            THORNS     = 8,  // +10% damage reflection
            KNIGHT     = 10, // +10% defense
            FORTITUDE  = 5,  // +20% defense
            RESILIENCE = 2   // +30% defense
        };

        Prefix prefix;
        Suffix suffix;

        
        Armor();
        /**
         * @brief Constructor for the Armor class
         * @param armType The type of armor
         * @param level The level of the player
         * @param add The amount to add to the level when calculating
         * @param crafted Whether the armor is crafted or not
         */
        Armor(const TYPE armType, const uint16_t level, const short add = 1, const bool crafted = false);
        
        static short getStats(const TYPE armType, const uint16_t level, const short add = 1);
        uint16_t levelUp() override;
        void displayInfo(const Source source = Source::NONE) const override;
    
    };
    
    class Weapon : public Leveled {
    public:
        short strengthBonus;

        enum class Prefix {
            NONE      = 50,
            CURSED    = 9,  // -20% strength
            DULL      = 16, // -10% strength
            SHARP     = 15, // +10% strength
            LEGENDARY = 10  // +20% strength
        };
    
        enum class Suffix {
            NONE        = 75,
            INFERNO     = 8,  // +1d4 fire damage
            VENGEANCE   = 10, // +10% strength
            SLAYER      = 5,  // +20% strength
            EXECUTIONER = 2   // +30% strength
        };

        Prefix prefix;
        Suffix suffix;

        Weapon();
    
        /**
         * @brief Constructor for the Weapon class
         * @param wpnType The type of weapon
         * @param level The level of the player
         * @param add The amount to add to the level when calculating
         * @param crafted Whether the weapon is crafted or not
         */
        Weapon(const TYPE wpnType, const uint16_t level, const short add = 1, const bool crafted = false);

        static short getStats(const TYPE wpnType, const uint16_t level, const short add = 1);
        uint16_t levelUp() override;
        void displayInfo(const Source source = Source::NONE) const override;
    
    };
    
    class Special : public Item {
    public:
        short defenseBonus,
              strengthBonus;

        Special();
    
        /**
         * @brief Constructor for the Special class
         * @param splType The type of special item
         * @param level The level of the player
         */
        Special(const TYPE splType, const uint16_t level);
    
        void displayInfo(const Source source = Source::NONE) const override;
        
    };

}