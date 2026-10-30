//CTask
#pragma once
#include<cstdint>
#include<string>

#ifndef ui16
    #define ui16(x) static_cast<uint16_t>(x)
#endif

typedef std::initializer_list<std::pair<std::string, uint16_t>> PairList_t;

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

    enum class AWClass { // Armor/Weapon Class
        LIGHT,
        HEAVY,
        RANGE,
        MELEE,
        NONE
    };

    enum class TYPE {
        NONE,
        POTION,
        FOCUS,
        DRP_CURSED_DST,
        DRP_BEAST_FANG,
        DRP_SHADOW_ESS,
        DRP_CHITIN,
        DRP_HIDE,
        ARM_LEATHER,
        ARM_DRAKONIAN,
        ARM_IRON,
        ARM_STEEL,
        ARM_CHITIN,
        ARM_HIDE,
        ARM_SHAD_COWL,
        WPN_LONG,
        WPN_MAGIC,
        WPN_GREAT,
        WPN_CROSSBOW,
        WPN_BEAST_BLADE,
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

    struct Info {
        ItemClass type;
        AWClass awClass;
        std::string_view name;
        PairList_t components = {};
        float defMod = 0.f;
        float strMod = 0.f;
        bool canHavePrefix = true;
        bool nDef = false;
        bool nStr = false;
    };

    const Info Data[] {
        { ItemClass::GEN, (AWClass)0    , "None"                                                                                                                                          },
        { ItemClass::GEN, (AWClass)0    , "Health Potion"               , { { "Medicinal Herbs", ui16(2) } }                                                                              },
        { ItemClass::GEN, (AWClass)0    , "Arcane Focus"                , { { "Crystals", ui16(4) } }                                                                                     },
        { ItemClass::GEN, (AWClass)0    , "Cursed Dust"                                                                                                                                   },
        { ItemClass::GEN, (AWClass)0    , "Beast Fang"                                                                                                                                    },
        { ItemClass::GEN, (AWClass)0    , "Shadow Essence"                                                                                                                                },
        { ItemClass::GEN, (AWClass)0    , "Chitin"                                                                                                                                        },
        { ItemClass::GEN, (AWClass)0    , "Hide"                                                                                                                                          },

        { ItemClass::ARM, AWClass::LIGHT, "Leather Armor"               , { { "Fiber", ui16(2) }, { "Leather", ui16(4) } }, 1.15f                                                         },
        { ItemClass::ARM, AWClass::LIGHT, "Drakonian Armor"             , { { "Fiber", ui16(2) }, { "Leather", ui16(4) } }, 1.10f                                                         },
        { ItemClass::ARM, AWClass::HEAVY, "Iron Armor"                  , {}, 1.20f                                                                                                       },
        { ItemClass::ARM, AWClass::HEAVY, "Steel Armor"                 , {}, 1.30f                                                                                                       },
        { ItemClass::ARM, AWClass::HEAVY, "Chitin Armor"                , { { "Fiber", ui16(2) }, { "Leather", ui16(2) }, { "Chitin", ui16(2) } }, 1.40f                                  },
        { ItemClass::ARM, AWClass::LIGHT, "Hide Armor"                  , { { "Fiber", ui16(2) }, { "Hide", ui16(2) } }, 1.25f                                                            },
        { ItemClass::ARM, AWClass::LIGHT, "Shadow's Cowl"               , { { "Shadow Essence", ui16(2) }, { "Cursed Dust", ui16(1) } }, 1.10f                                            },

        { ItemClass::WPN, AWClass::MELEE, "Longsword"                   , { { "Fiber", ui16(2) }, { "Iron", ui16(3) }, { "Wood", ui16(2) } }, 0.00f, 1.25f                                },
        { ItemClass::WPN, AWClass::MELEE, "Magic Sword"                 , {}, 0.00f, 1.25f                                                                                                },
        { ItemClass::WPN, AWClass::MELEE, "Greatsword"                  , {}, 0.00f, 1.25f                                                                                                },
        { ItemClass::WPN, AWClass::RANGE, "Crossbow"                    , {}, 0.00f, 1.25f                                                                                                },
        { ItemClass::WPN, AWClass::MELEE, "Beast Blade"                 , { { "Fiber", ui16(2) }, { "Wood", ui16(2) }, { "Beast Fang", ui16(1) } }, 0.00f, 1.40f                          },
        { ItemClass::WPN, AWClass::MELEE, "Staff of the Warborn"        , { { "Amulet of the Warborn", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }, 0.00f, 1.25f, false        },
        { ItemClass::WPN, AWClass::MELEE, "Staff of the Guardian"       , { { "Amulet of the Guardian", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }, 0.00f, 1.25f, false       },
        { ItemClass::WPN, AWClass::MELEE, "Staff of the Shadow"         , { { "Amulet of the Shadow", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }, 0.00f, 1.25f, false         },
        { ItemClass::WPN, AWClass::MELEE, "Staff of Fury"               , { { "Amulet of Fury", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }, 0.00f, 1.35f, false               },
        { ItemClass::WPN, AWClass::MELEE, "Staff of the Weeping Spirit" , { { "Amulet of the Weeping Spirit", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }, 0.00f, 1.10f, false },

        { ItemClass::SPL, (AWClass)0    , "Amulet of the Warborn"       , {}, 1.20f, 1.20f                                                                                                },
        { ItemClass::SPL, (AWClass)0    , "Amulet of the Guardian"      , {}, 1.30f, 0.00f                                                                                                },
        { ItemClass::SPL, (AWClass)0    , "Amulet of the Shadow"        , {}, 0.00f, 0.00f                                                                                                },
        { ItemClass::SPL, (AWClass)0    , "Amulet of Fury"              , {}, 1.25f, 1.25f, false, true                                                                                   },
        { ItemClass::SPL, (AWClass)0    , "Amulet of the Weeping Spirit", {}, 1.20f, 1.20f, false, true, true                                                                             }
    };

    constexpr std::size_t toIdx(TYPE type) {
        return static_cast<std::size_t>(type);
    }

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
        AWClass iClass;

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
        AWClass iClass;

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
