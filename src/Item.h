#pragma once
#include<unordered_map>
#include<string>
#include"Util.h"
//import Util;

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
        std::string name;
        float defMod = 0.f;
        float strMod = 0.f;
        bool nDef = false;
        bool nStr = false;
    };

    const std::unordered_map<TYPE, Info> Data = {
        // General
        { TYPE::NONE           , { "None"                                                   }},
        { TYPE::FOCUS          , { "Arcane Focus"                                           }},
        // Armor
        { TYPE::ARM_LEATHER    , { "Leather Armor"               , 1.15f                    }},
        { TYPE::ARM_DRAKONIAN  , { "Drakonian Armor"             , 1.15f                    }},
        { TYPE::ARM_IRON       , { "Iron Armor"                  , 1.20f                    }},
        { TYPE::ARM_STEEL      , { "Steel Armor"                 , 1.30f                    }},
        // Weapons
        { TYPE::WPN_LONG       , { "Longsword"                   , 0.00f, 1.25f             }},
        { TYPE::WPN_MAGIC      , { "Magic Sword"                 , 0.00f, 1.25f             }},
        { TYPE::WPN_GREAT      , { "Greatsword"                  , 0.00f, 1.25f             }},
        { TYPE::WPN_CROSSBOW   , { "Crossbow"                    , 0.00f, 1.25f             }},
        { TYPE::WPN_ST_WARBORN , { "Staff of the Warborn"        , 0.00f, 1.25f             }},
        { TYPE::WPN_ST_GUARDIAN, { "Staff of the Guardian"       , 0.00f, 1.25f             }},
        { TYPE::WPN_ST_SHADOW  , { "Staff of the Shadow"         , 0.00f, 1.25f             }},
        { TYPE::WPN_ST_FURY    , { "Staff of Fury"               , 0.00f, 1.35f             }},
        { TYPE::WPN_ST_WEEPING , { "Staff of the Weeping Spirit" , 0.00f, 1.10f             }},
        // Special
        { TYPE::SPL_AM_WARBORN , { "Amulet of the Warborn"       , 1.20f, 1.20f             }},
        { TYPE::SPL_AM_GUARDIAN, { "Amulet of the Guardian"      , 1.30f                    }},
        { TYPE::SPL_AM_SHADOW  , { "Amulet of the Shadow"                                   }},
        { TYPE::SPL_AM_FURY    , { "Amulet of Fury"              , 1.25f, 1.25f, true       }},
        { TYPE::SPL_AM_WEEPING , { "Amulet of the Weeping Spirit", 1.20f, 1.20f, true, true }}
    };

    class Item {
    public:
        std::string name;
        TYPE itemType;
        
        Item(const TYPE t = TYPE::NONE) : itemType(t), name(Data.at(t).name) {}

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
        
        const bool operator!=(const TYPE Type) const {
            return itemType != Type;
        }
        
        const bool operator==(const TYPE Type) const {
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
    
        Armor(const TYPE t, const uint16_t l, const short a = 1, const bool c = false) :
            Leveled(t), defenseBonus(c ? std::round(pow(l + a, Data.at(t).defMod)) : randint(1, std::round(pow(l + a, Data.at(t).defMod)))) {}
    
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
    
        Weapon(const TYPE t, const uint16_t l, const short a = 1, const bool c = false) :
            Leveled(t), strengthBonus(c ? std::abs(std::round(pow(l + a, Data.at(t).strMod))) : randint(1, std::abs(std::round(pow(l + a, Data.at(t).strMod))))) {}
    
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
    
        Special(const TYPE t, const uint16_t l) : Item(t),
            defenseBonus(Data.at(t).defMod == 0 ? 0 : randint(1, std::round(pow(l, Data.at(t).defMod))) * (Data.at(t).nDef ? -1 : 1)),
            strengthBonus(Data.at(t).strMod == 0 ? 0 : randint(1, std::round(pow(l, Data.at(t).strMod))) * (Data.at(t).nStr ? -1 : 1)) {}
    
        void displayInfo(const Source source = Source::NONE) const {
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