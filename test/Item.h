#pragma once
#include<unordered_map>
#include<string>
#include"Util.h"

namespace Item {

    enum Source {
        FIND,
        CRAFT
    };

    class Potion {
    public:
        std::string name;
    
        Potion(std::string n) : name(n) {}
    
        void displayInfo(const Item::Source source = Item::FIND) const {
            
        }
    
    };
    
    class Leveled {
    public:
        std::string name;
        uint16_t level;
        
        Leveled() {}

        Leveled(std::string n) : name(n), level(1) {}
    
        void levelUp() {
            level++;
        }
    
        virtual void displayInfo(const std::string source = "") const {}
    
    };
    
    class Armor : public Leveled {
    public:
        short defenseBonus;
    
        enum TYPE {
            NONE,
            LEATHER,
            DRAKONIAN,
            IRON,
            STEEL,
            COUNT
        };
    
        struct Info {
            std::string name;
            float defMod;
        };

        inline static const Info aType[COUNT] = {
            { "None",            0.f   },
            { "Leather Armor",   1.15f },
            { "Drakonian Armor", 1.15f },
            { "Iron Armor",      1.2f  },
            { "Steel Armor",     1.3f  }
        };

        Armor() : Leveled("None"), defenseBonus(0) {}
    
        Armor(const TYPE t, const uint16_t l, const uint16_t a = 1, const bool c = false) :
            Leveled(aType[t].name), defenseBonus(c ? std::round(pow(l + a, aType[t].defMod)) : randint(1, std::round(pow(l + a, aType[t].defMod)))) {}
    
        void displayInfo(const std::string source = "") const override {
            if (source == "find")
                type("\nYou found an item: ");
            else if (source == "craft")
                type("You crafted an item: ");
            else if (source == "drop")
                type("The enemy dropped an item: ");
            type(name, " (Defense Bonus: ", defenseBonus, ", Attack Bonus: 0)\n");
        }

        const bool operator!=(const TYPE armType) const {
            return name != aType[armType].name;
        }

        const bool operator==(const TYPE armType) const {
            return name == aType[armType].name;
        }
    
    };
    
    class Weapon : public Leveled {
    public:
        short attackBonus;

        enum TYPE {
            NONE,
            LONG,
            MAGIC,
            GREAT,
            CROSSBOW,
            COUNT
        };
    
        struct Info {
            std::string name;
            float atkMod;
        };

        inline static const Info wType[COUNT] = {
            { "None",        0.f   },
            { "Longsword",   1.25f },
            { "Magic Sword", 1.3f  },
            { "Greatsword",  1.3f  },
            { "Crossbow",    1.3f  }
        };

        Weapon() : Leveled("None"), attackBonus(0) {}
    
        Weapon(const TYPE t, const uint16_t l, const uint16_t a = 1, const bool c = false) :
            Leveled(wType[t].name), attackBonus(c ? std::round(pow(l + a, wType[t].atkMod)) : randint(1, std::round(pow(l + a, wType[t].atkMod)))) {}
    
        void displayInfo(const std::string source = "") const override {
            if (source == "find")
                type("\nYou found an item: ");
            else if (source == "craft")
                type("You crafted an item: ");
            else if (source == "drop")
                type("The enemy dropped an item: ");
            type(name, " (Defense Bonus: 0, Attack Bonus: ", attackBonus, ")\n");
        }

        const bool operator!=(const TYPE wpnType) {
            return name != wType[wpnType].name;
        }

        const bool operator==(const TYPE wpnType) {
            return name == wType[wpnType].name;
        }
    
    };
    
    class Special {
    public:
        std::string name;
        short defenseBonus,
              attackBonus;

        enum TYPE {
            NONE,
            A_WARBORN,
            A_GUARDIAN,
            A_SHADOW,
            A_FURY,
            A_WEEPING,
            COUNT
        };

        struct Info {
            std::string name;
            float defMod,
                  atkMod;
            bool nDef,
                 nAtk;
        };

        inline static const Info sType[COUNT] = {
            { "None",                           0.f, 0.f, false, false },
            { "Amulet of the Warborn",        1.2f, 1.2f, false, false },
            { "Amulet of the Guardian",        1.3f, 0.f, false, false },
            { "Amulet of the Shadow",           0.f, 0.f, false, false },
            { "Amulet of Fury",             1.25f, 1.25f, true,  false },
            { "Amulet of the Weeping Spirit", 1.2f, 1.2f, true,  true  }
        };

        Special() : name("None"), defenseBonus(0), attackBonus(0) {}
    
        Special(const TYPE t, const uint16_t l) : name(sType[t].name),
            defenseBonus(sType[t].defMod == 0 ? 0 : randint(1, std::round(pow(l, sType[t].defMod))) * (sType[t].nDef ? -1 : 1)),
            attackBonus (sType[t].atkMod == 0 ? 0 : randint(1, std::round(pow(l, sType[t].atkMod))) * (sType[t].nAtk ? -1 : 1)) {}
    
        void displayInfo(const std::string source = "") const {
            if (source == "find")
                type("\nYou found an item: ");
            else if (source == "craft")
                type("\nYou crafted an item: ");
            else if (source == "drop")
                type("The enemy dropped an item: ");
            type(name, " (Defense Bonus: ", defenseBonus, ", Attack Bonus: ", attackBonus, ")\n");
        }

        const bool operator!=(const TYPE splType) {
            return name != sType[splType].name;
        }

        const bool operator==(const TYPE splType) {
            return name == sType[splType].name;
        }
        
    };
}