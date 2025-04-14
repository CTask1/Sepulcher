//CTask
#pragma once
#include<string_view>

#include"consts.h"
#include"Util.h"

namespace Item {

    enum Source {
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

        { ItemClass::SPL, "Amulet of the Warborn"       , 1.20f, 1.20f,                   },
        { ItemClass::SPL, "Amulet of the Guardian"      , 1.30f, 0.00f,                   },
        { ItemClass::SPL, "Amulet of the Shadow"        , 0.00f, 0.00f,                   },
        { ItemClass::SPL, "Amulet of Fury"              , 1.25f, 1.25f, false, true       },
        { ItemClass::SPL, "Amulet of the Weeping Spirit", 1.20f, 1.20f, false, true, true }
    };

    class Item {
    public:
        std::string name;
        TYPE itemType;

        Item(const TYPE type = TYPE::NONE) : itemType(type), name(Data[static_cast<uint16_t>(type)].name) {}

        virtual ~Item() {}
        
        virtual void displayInfo(const Source source = Source::NONE) const {
            if (source == FIND)
                type("\nYou found an item: ");
            else if (source == CRAFT)
                type("You crafted an item: ");
            else if (source == DROP)
                type("The enemy dropped an item: ");
            type(name, "\n");
        }
        
        bool operator!=(const TYPE Type) const {
            return itemType != Type;
        }
        
        bool operator==(const TYPE Type) const {
            return itemType == Type;
        }

    };
    
    class Leveled : public Item {
    public:
        uint16_t exp;
        uint16_t level;
        uint16_t nextLevel;

        Leveled(const TYPE t = TYPE::NONE) : Item(t), exp(0), level(1), nextLevel(11) {}
    
        virtual uint16_t levelUp() {
            level++;
            nextLevel = 10 + static_cast<uint16_t>(pow(level, 2)); // 10 + level to the power of 2
            exp = 0; // reset experience
            type ("\nYour ", name, " leveled up! It is now level ", level, ".\n");
            return 0;
        }
    
        virtual void displayInfo(const Source source = Source::NONE) const {
            if (source == FIND)
                type("\nYou found an item: ");
            else if (source == CRAFT)
                type("You crafted an item: ");
            else if (source == DROP)
                type("The enemy dropped an item: ");
            type(name, "\n");
        }
    
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

        static short getStats(const TYPE armType, const uint16_t level, const short add = 1) {
            return (short)std::round(pow(level + add, Data[static_cast<uint16_t>(armType)].defMod));
        }

        Armor() : defenseBonus(0) {}
    
        /**
         * @brief Constructor for the Armor class
         * @param armType The type of armor
         * @param level The level of the player
         * @param add The amount to add to the level when calculating
         * @param crafted Whether the armor is crafted or not
         */
        Armor(const TYPE armType, const uint16_t level, const short add = 1, const bool crafted = false) :
            Leveled(armType),
            defenseBonus (crafted // if the item is crafted
                ? (short)std::round(pow(level + add, Data[static_cast<uint16_t>(armType)].defMod)) // (level + add) to the power of defMod
                : (short)randint(1, (uint16_t)std::round(pow(level + add, Data[static_cast<uint16_t>(armType)].defMod))) // random number between 1 and the value above
            ),
            prefix(Prefix::NONE),
            suffix(Suffix::NONE) {
                // Set prefix
                uint16_t num = randint(1, 100);
                uint16_t prb = u(Prefix::NONE);
                if (num > prb && num <= prb + u(Prefix::RUSTED)) {
                    name = "Rusted " + name;
                    prefix = Prefix::RUSTED;
                    defenseBonus = (short)(defenseBonus * 0.8f);
                }
                prb += u(Prefix::RUSTED);
                if (num > prb && num <= prb + u(Prefix::WORN)) {
                    name = "Worn " + name;
                    prefix = Prefix::WORN;
                    defenseBonus = (short)(defenseBonus * 0.9f);
                }
                prb += u(Prefix::WORN);
                if (num > prb && num <= prb + u(Prefix::HEAVY)) {
                    name = "Heavy " + name;
                    prefix = Prefix::HEAVY;
                    defenseBonus = (short)(defenseBonus * 1.1f);
                }
                prb += u(Prefix::HEAVY);
                if (num > prb && num <= prb + u(Prefix::ENCHANTED)) {
                    name = "Enchanted " + name;
                    prefix = Prefix::ENCHANTED;
                    defenseBonus = (short)(defenseBonus * 1.2f);
                }

                // Set suffix
                num = randint(1, 100);
                prb = u(Suffix::NONE);
                if (num > prb && num <= prb + u(Suffix::THORNS)) {
                    name += " of Thorns";
                    suffix = Suffix::THORNS;
                    return;
                }
                prb += u(Suffix::THORNS);
                if (num > prb && num <= prb + u(Suffix::KNIGHT)) {
                    name += " of the Knight";
                    suffix = Suffix::KNIGHT;
                    defenseBonus = (short)(defenseBonus * 1.1f);
                    return;
                }
                prb += u(Suffix::KNIGHT);
                if (num > prb && num <= prb + u(Suffix::FORTITUDE)) {
                    name += " of Fortitude";
                    suffix = Suffix::FORTITUDE;
                    defenseBonus = (short)(defenseBonus * 1.2f);
                    return;
                }
                prb += u(Suffix::FORTITUDE);
                if (num > prb && num <= prb + u(Suffix::RESILIENCE)) {
                    name += " of Resilience";
                    suffix = Suffix::RESILIENCE;
                    defenseBonus = (short)(defenseBonus * 1.3f);
                    return;
                }
            }

        uint16_t levelUp() override {
            uint16_t bonus = 0;
            if (exp >= nextLevel) {
                bonus = (short)std::round(pow(level, Data[static_cast<uint16_t>(itemType)].defMod / 2)); // level to the power of defMod / 2
                defenseBonus += bonus;
                Leveled::levelUp();
            }
            return bonus;
        }
    
        void displayInfo(const Source source = Source::NONE) const override {
            if (source == FIND)
                type("\nYou found an item: ");
            else if (source == CRAFT)
                type("You crafted an item: ");
            else if (source == DROP)
                type("The enemy dropped an item: ");
            type(name, " (Defense Bonus: ", defenseBonus, ")\n");
        }
    
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

        static short getStats(const TYPE wpnType, const uint16_t level, const short add = 1) {
            return (short)std::abs(std::round(pow(level + add, Data[static_cast<uint16_t>(wpnType)].strMod)));
        }

        Weapon() : strengthBonus(0) {}
    
        /**
         * @brief Constructor for the Weapon class
         * @param wpnType The type of weapon
         * @param level The level of the player
         * @param add The amount to add to the level when calculating
         * @param crafted Whether the weapon is crafted or not
         */
        Weapon(const TYPE wpnType, const uint16_t level, const short add = 1, const bool crafted = false) :
            Leveled(wpnType),
            strengthBonus (crafted // if the item is crafted
                ? (short)std::abs(std::round(pow(level + add, Data[static_cast<uint16_t>(wpnType)].strMod))) // (level + add) to the power of strMod
                : (short)randint(1, (uint16_t)std::abs(std::round(pow(level + add, Data[static_cast<uint16_t>(wpnType)].strMod)))) // random number between 1 and the value above
            ),
            prefix(Prefix::NONE),
            suffix(Suffix::NONE) {
                uint16_t num;
                uint16_t prb;
                // Set prefix
                if (Data[static_cast<uint16_t>(wpnType)].canHavePrefix) {
                    num = randint(1, 100);
                    prb = u(Prefix::NONE);
                    if (num > prb && num <= prb + u(Prefix::CURSED)) {
                        name = "Cursed " + name;
                        prefix = Prefix::CURSED;
                        strengthBonus = (short)(strengthBonus * 0.8f);
                    }
                    prb += u(Prefix::CURSED);
                    if (num > prb && num <= prb + u(Prefix::DULL)) {
                        name = "Dull " + name;
                        prefix = Prefix::DULL;
                        strengthBonus = (short)(strengthBonus * 0.9f);
                    }
                    prb += u(Prefix::DULL);
                    if (num > prb && num <= prb + u(Prefix::SHARP)) {
                        name = "Sharp " + name;
                        prefix = Prefix::SHARP;
                        strengthBonus = (short)(strengthBonus * 1.1f);
                    }
                    prb += u(Prefix::SHARP);
                    if (num > prb && num <= prb + u(Prefix::LEGENDARY)) {
                        name = "Legendary " + name;
                        prefix = Prefix::LEGENDARY;
                        strengthBonus = (short)(strengthBonus * 1.2f);
                    }
                }

                // Set suffix
                num = randint(1, 100);
                prb = u(Suffix::NONE);
                if (num > prb && num <= prb + u(Suffix::INFERNO)) {
                    name += " of the Inferno";
                    suffix = Suffix::INFERNO;
                    return;
                }
                prb += u(Suffix::INFERNO);
                if (num > prb && num <= prb + u(Suffix::VENGEANCE)) {
                    name += " of Vengeance";
                    suffix = Suffix::VENGEANCE;
                    strengthBonus = (short)(strengthBonus * 1.1f);
                    return;
                }
                prb += u(Suffix::VENGEANCE);
                if (num > prb && num <= prb + u(Suffix::SLAYER)) {
                    name += " of the Slayer";
                    suffix = Suffix::SLAYER;
                    strengthBonus = (short)(strengthBonus * 1.2f);
                    return;
                }
                prb += u(Suffix::SLAYER);
                if (num > prb && num <= prb + u(Suffix::EXECUTIONER)) {
                    name += " of the Executioner";
                    suffix = Suffix::EXECUTIONER;
                    strengthBonus = (short)(strengthBonus * 1.3f);
                    return;
                }
            }

        uint16_t levelUp() override {
            uint16_t bonus = 0;
            if (exp >= nextLevel) {
                bonus = (short)std::round(pow(level, Data[static_cast<uint16_t>(itemType)].strMod / 2)); // level to the power of strMod / 2
                strengthBonus += bonus;
                Leveled::levelUp();
            }
            return bonus;
        }
    
        void displayInfo(const Source source = Source::NONE) const override {
            if (source == FIND)
                type("\nYou found an item: ");
            else if (source == CRAFT)
                type("You crafted an item: ");
            else if (source == DROP)
                type("The enemy dropped an item: ");
            type(name, " (Strength Bonus: ", strengthBonus, ")\n");
        }
    
    };
    
    class Special : public Item {
    public:
        short defenseBonus,
              strengthBonus;

        Special() : defenseBonus(0), strengthBonus(0) {}
    
        /**
         * @brief Constructor for the Special class
         * @param splType The type of special item
         * @param level The level of the player
         */
        Special(const TYPE splType, const uint16_t level) :
            Item(splType),
            defenseBonus (Data[static_cast<uint16_t>(splType)].defMod == 0 // if defMod is 0
                ? 0
                : (short)randint(1, (uint16_t)std::round(pow(level, Data[static_cast<uint16_t>(splType)].defMod))) * // a random number between 1 and the level to the power of defMod
                (Data[static_cast<uint16_t>(splType)].nDef // ...multiplied by -1 if nDef is true
                    ? -1
                    : 1
                )
            ),
            strengthBonus (Data[static_cast<uint16_t>(splType)].strMod == 0 // if strMod is 0
                ? 0
                : (short)randint(1, (uint16_t)std::round(pow(level, Data[static_cast<uint16_t>(splType)].strMod))) * // a random number between 1 and the level to the power of strMod
                (Data[static_cast<uint16_t>(splType)].nStr // ...multiplied by -1 if nStr is true
                    ? -1
                    : 1
                )
            ) {}
    
        void displayInfo(const Source source = Source::NONE) const override {
            if (source == FIND)
                type("\nYou found an item: ");
            else if (source == CRAFT)
                type("\nYou crafted an item: ");
            else if (source == DROP)
                type("The enemy dropped an item: ");
            type(name, " (Defense Bonus: ", defenseBonus, ", Strength Bonus: ", strengthBonus, ")\n");
        }
        
    };
}