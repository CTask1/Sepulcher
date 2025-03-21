#pragma once
#include<unordered_map>
#include<string>
#include"Util.h"

namespace Item {

    enum Source {
        NONE,
        FIND,
        CRAFT,
        DROP
    };

    enum class TYPE {
        NONE,
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
        std::string_view name;
        float defMod = 0.f;
        float strMod = 0.f;
        bool nDef = false;
        bool nStr = false;
    };

    constexpr Info Data[] {
        // General
        { "None"                                                   },
        { "Arcane Focus"                                           },
        // Armor
        { "Leather Armor"               , 1.15f                    },
        { "Drakonian Armor"             , 1.15f                    },
        { "Iron Armor"                  , 1.20f                    },
        { "Steel Armor"                 , 1.30f                    },
        // Weapons
        { "Longsword"                   , 0.00f, 1.25f             },
        { "Magic Sword"                 , 0.00f, 1.25f             },
        { "Greatsword"                  , 0.00f, 1.25f             },
        { "Crossbow"                    , 0.00f, 1.25f             },
        { "Staff of the Warborn"        , 0.00f, 1.25f             },
        { "Staff of the Guardian"       , 0.00f, 1.25f             },
        { "Staff of the Shadow"         , 0.00f, 1.25f             },
        { "Staff of Fury"               , 0.00f, 1.35f             },
        { "Staff of the Weeping Spirit" , 0.00f, 1.10f             },
        // Special
        { "Amulet of the Warborn"       , 1.20f, 1.20f             },
        { "Amulet of the Guardian"      , 1.30f                    },
        { "Amulet of the Shadow"                                   },
        { "Amulet of Fury"              , 1.25f, 1.25f, true       },
        { "Amulet of the Weeping Spirit", 1.20f, 1.20f, true, true }
    };

    class Item {
    public:
        std::string_view name;
        TYPE itemType;
        
        Item(const TYPE t = TYPE::NONE) : itemType(t), name(Data[static_cast<uint16_t>(t)].name) {}

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
    
    class Potion : public Item {
    public:
    
        Potion(const TYPE t = TYPE::NONE) : Item(t) {}
    
        void displayInfo(const Source source = Source::NONE) const {
            
        }
    
    };
    
    class Leveled : public Item {
    public:
        uint16_t level;

        Leveled(const TYPE t = TYPE::NONE) : Item(t), level(1) {}
    
        void levelUp() {
            level++;
        }
    
        virtual void displayInfo(const Source source = Source::NONE) const {}
    
    };
    
    class Armor : public Leveled {
    public:
        short defenseBonus;

        Armor() : defenseBonus(0) {}
    
        Armor(const TYPE armType, const uint16_t level, const short add = 1, const bool crafted = false) :
            Leveled(armType),
            defenseBonus (crafted
                ? std::round(pow(level + add, Data[static_cast<uint16_t>(armType)].defMod))
                : randint(1, std::round(pow(level + add, Data[static_cast<uint16_t>(armType)].defMod)))
            ) {}
    
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

        Weapon() : strengthBonus(0) {}
    
        Weapon(const TYPE wpnType, const uint16_t level, const short add = 1, const bool crafted = false) :
            Leveled(wpnType),
            strengthBonus (crafted
                ? std::abs(std::round(pow(level + add, Data[static_cast<uint16_t>(wpnType)].strMod)))
                : randint(1, std::abs(std::round(pow(level + add, Data[static_cast<uint16_t>(wpnType)].strMod))))
            ) {}
    
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
    
        Special(const TYPE splType, const uint16_t level) :
            Item(splType),
            defenseBonus (Data[static_cast<uint16_t>(splType)].defMod == 0
                ? 0
                : randint(1, std::round(pow(level, Data[static_cast<uint16_t>(splType)].defMod))) *
                (Data[static_cast<uint16_t>(splType)].nDef
                    ? -1
                    : 1
                )
            ),
            strengthBonus (Data[static_cast<uint16_t>(splType)].strMod == 0
                ? 0
                : randint(1, std::round(pow(level, Data[static_cast<uint16_t>(splType)].strMod))) *
                (Data[static_cast<uint16_t>(splType)].nStr
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