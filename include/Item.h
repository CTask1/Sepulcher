//CTask
#pragma once
#include<string_view>

#include"util.h"

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

        Item(const TYPE type = TYPE::NONE) : itemType(type), name(Data[ui16(type)].name) {}

        virtual ~Item() = default;
        
        virtual void displayInfo(const Source source = Source::NONE) const {
            if (source == FIND)
                type("\nYou found an item: ");
            else if (source == CRAFT)
                type("\nYou crafted an item: ");
            else if (source == DROP)
                type("\nThe enemy dropped an item: ");
            type(name);
        }
        
        bool operator!=(const TYPE Type) const {
            return itemType != Type;
        }
        
        bool operator==(const TYPE Type) const {
            return itemType == Type;
        }

    };

    class General : public Item {
    public:

        General(const TYPE t = TYPE::NONE) : Item(t) {}
    
        void displayInfo(const Source source = Source::NONE) const override {
            Item::displayInfo(source);
            type("\n");
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
            nextLevel = 10 + ui16(pow(level, 2)); // 10 + level to the power of 2
            exp = 0; // reset experience
            type ("\nYour ", name, " leveled up! It is now level ", level, ".\n");
            return 0;
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
            return shrt(std::round(pow(level + add, Data[ui16(armType)].defMod)));
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
                ? shrt(std::round(pow(level + add, Data[ui16(armType)].defMod))) // (level + add) to the power of defMod
                : shrt(randint(1, ui16(std::round(pow(level + add, Data[ui16(armType)].defMod))))) // random number between 1 and the value above
            ),
            prefix(Prefix::NONE),
            suffix(Suffix::NONE) {
                // Set prefix
                uint16_t num = randint(1, 100);
                uint16_t prb = ui16(Prefix::NONE);
                if (num > prb && num <= prb + ui16(Prefix::RUSTED)) {
                    name = "Rusted " + name;
                    prefix = Prefix::RUSTED;
                    defenseBonus = shrt(defenseBonus * 0.8f);
                }
                prb += ui16(Prefix::RUSTED);
                if (num > prb && num <= prb + ui16(Prefix::WORN)) {
                    name = "Worn " + name;
                    prefix = Prefix::WORN;
                    defenseBonus = shrt(defenseBonus * 0.9f);
                }
                prb += ui16(Prefix::WORN);
                if (num > prb && num <= prb + ui16(Prefix::HEAVY)) {
                    name = "Heavy " + name;
                    prefix = Prefix::HEAVY;
                    defenseBonus = shrt(defenseBonus * 1.1f);
                }
                prb += ui16(Prefix::HEAVY);
                if (num > prb && num <= prb + ui16(Prefix::ENCHANTED)) {
                    name = "Enchanted " + name;
                    prefix = Prefix::ENCHANTED;
                    defenseBonus = shrt(defenseBonus * 1.2f);
                }

                // Set suffix
                num = randint(1, 100);
                prb = ui16(Suffix::NONE);
                if (num > prb && num <= prb + ui16(Suffix::THORNS)) {
                    name += " of Thorns";
                    suffix = Suffix::THORNS;
                    return;
                }
                prb += ui16(Suffix::THORNS);
                if (num > prb && num <= prb + ui16(Suffix::KNIGHT)) {
                    name += " of the Knight";
                    suffix = Suffix::KNIGHT;
                    defenseBonus = shrt(defenseBonus * 1.1f);
                    return;
                }
                prb += ui16(Suffix::KNIGHT);
                if (num > prb && num <= prb + ui16(Suffix::FORTITUDE)) {
                    name += " of Fortitude";
                    suffix = Suffix::FORTITUDE;
                    defenseBonus = shrt(defenseBonus * 1.2f);
                    return;
                }
                prb += ui16(Suffix::FORTITUDE);
                if (num > prb && num <= prb + ui16(Suffix::RESILIENCE)) {
                    name += " of Resilience";
                    suffix = Suffix::RESILIENCE;
                    defenseBonus = shrt(defenseBonus * 1.3f);
                    return;
                }
            }

        uint16_t levelUp() override {
            uint16_t bonus = 0;
            if (exp >= nextLevel) {
                bonus = shrt(std::round(pow(level, Data[ui16(itemType)].defMod / 2))); // level to the power of defMod / 2
                defenseBonus += bonus;
                Leveled::levelUp();
            }
            return bonus;
        }
    
        void displayInfo(const Source source = Source::NONE) const override {
            Item::displayInfo(source);
            type(" (Defense Bonus: ", defenseBonus, ")\n");
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
            return shrt(std::abs(std::round(pow(level + add, Data[ui16(wpnType)].strMod))));
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
                ? shrt(std::abs(std::round(pow(level + add, Data[ui16(wpnType)].strMod)))) // (level + add) to the power of strMod
                : shrt(randint(1, ui16(std::abs(std::round(pow(level + add, Data[ui16(wpnType)].strMod)))))) // random number between 1 and the value above
            ),
            prefix(Prefix::NONE),
            suffix(Suffix::NONE) {
                uint16_t num;
                uint16_t prb;
                // Set prefix
                if (Data[ui16(wpnType)].canHavePrefix) {
                    num = randint(1, 100);
                    prb = ui16(Prefix::NONE);
                    if (num > prb && num <= prb + ui16(Prefix::CURSED)) {
                        name = "Cursed " + name;
                        prefix = Prefix::CURSED;
                        strengthBonus = shrt(strengthBonus * 0.8f);
                    }
                    prb += ui16(Prefix::CURSED);
                    if (num > prb && num <= prb + ui16(Prefix::DULL)) {
                        name = "Dull " + name;
                        prefix = Prefix::DULL;
                        strengthBonus = shrt(strengthBonus * 0.9f);
                    }
                    prb += ui16(Prefix::DULL);
                    if (num > prb && num <= prb + ui16(Prefix::SHARP)) {
                        name = "Sharp " + name;
                        prefix = Prefix::SHARP;
                        strengthBonus = shrt(strengthBonus * 1.1f);
                    }
                    prb += ui16(Prefix::SHARP);
                    if (num > prb && num <= prb + ui16(Prefix::LEGENDARY)) {
                        name = "Legendary " + name;
                        prefix = Prefix::LEGENDARY;
                        strengthBonus = shrt(strengthBonus * 1.2f);
                    }
                }

                // Set suffix
                num = randint(1, 100);
                prb = ui16(Suffix::NONE);
                if (num > prb && num <= prb + ui16(Suffix::INFERNO)) {
                    name += " of the Inferno";
                    suffix = Suffix::INFERNO;
                    return;
                }
                prb += ui16(Suffix::INFERNO);
                if (num > prb && num <= prb + ui16(Suffix::VENGEANCE)) {
                    name += " of Vengeance";
                    suffix = Suffix::VENGEANCE;
                    strengthBonus = shrt(strengthBonus * 1.1f);
                    return;
                }
                prb += ui16(Suffix::VENGEANCE);
                if (num > prb && num <= prb + ui16(Suffix::SLAYER)) {
                    name += " of the Slayer";
                    suffix = Suffix::SLAYER;
                    strengthBonus = shrt(strengthBonus * 1.2f);
                    return;
                }
                prb += ui16(Suffix::SLAYER);
                if (num > prb && num <= prb + ui16(Suffix::EXECUTIONER)) {
                    name += " of the Executioner";
                    suffix = Suffix::EXECUTIONER;
                    strengthBonus = shrt(strengthBonus * 1.3f);
                    return;
                }
            }

        uint16_t levelUp() override {
            uint16_t bonus = 0;
            if (exp >= nextLevel) {
                bonus = shrt(std::round(pow(level, Data[ui16(itemType)].strMod / 2))); // level to the power of strMod / 2
                strengthBonus += bonus;
                Leveled::levelUp();
            }
            return bonus;
        }
    
        void displayInfo(const Source source = Source::NONE) const override {
            Item::displayInfo(source);
            type(" (Strength Bonus: ", strengthBonus, ")\n");
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
            defenseBonus (Data[ui16(splType)].defMod == 0 // if defMod is 0
                ? 0
                : shrt (
                    randint ( // a random number between 1 and the level to the power of defMod
                        1,
                        ui16(std::round(pow(level, Data[ui16(splType)].defMod)))
                    ) *
                    (Data[ui16(splType)].nDef // ...multiplied by -1 if nDef is true
                        ? -1
                        : 1
                    )
                )
            ),
            strengthBonus (Data[ui16(splType)].strMod == 0 // if strMod is 0
                ? 0
                : shrt (
                    randint ( // a random number between 1 and the level to the power of strMod
                        1,
                        ui16(std::round(pow(level, Data[ui16(splType)].strMod)))
                    ) *
                    (Data[ui16(splType)].nStr // ...multiplied by -1 if nStr is true
                        ? -1
                        : 1
                    )
                )
            ) {}
    
        void displayInfo(const Source source = Source::NONE) const override {
            Item::displayInfo(source);
            type(" (Defense Bonus: ", defenseBonus, ", Strength Bonus: ", strengthBonus, ")\n");
        }
        
    };
}