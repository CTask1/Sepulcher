#pragma once
#include"Resources.h"
#include<string>
#include<cmath>
#include"Util.h"
#include"Item.h"

class Player {
public:

    enum RACE {
        ELF,
        HUMAN,
        DRAKONIAN,
        RCOUNT
    };

    enum CLASS {
        FIGHTER,
        ROGUE,
        CCOUNT
    };

    inline static const struct RA {
            RACE Race;
            std::string name;
            const bool operator==(const RACE compRace) {
                return Race == compRace;
            }
            const bool operator!=(const RACE compRace) {
                return Race != compRace;
            }
        } R[RACE::RCOUNT] = {
            { ELF,       "Elf"       },
            { HUMAN,     "Human"     },
            { DRAKONIAN, "Drakonian" }
    };

    inline static const struct CL {
            CLASS Class;
            std::string name;
            const bool operator==(const CLASS compClass) {
                return Class == compClass;
            }
            const bool operator!=(const CLASS compClass) {
                return Class != compClass;
            }
        } C[CLASS::CCOUNT] = {
            { FIGHTER, "Fighter" },
            { ROGUE,   "Rogue"   }
    };

    std::string name;
    RA Race;
    CL Class;
    uint16_t maxhealth, health, baseSTR, CON, level;
    short STR, defense;
    uint32_t nextLevel, exp;
    Item::Weapon weapon;
    Item::Armor armor;
    Item::Special special;
    bool hasAbility;
    bool raceAbilityReady;
    bool classAbilityReady;
    Resources resources;

    Player(std::string n, RACE ra, CLASS cl, int h, int s, int co) :
        name(n), Race({R[ra].Race, R[ra].name}), Class({C[cl].Class, C[cl].name}),
        maxhealth(h), health(h), baseSTR(s), STR(s), CON(co), defense(0),
        level(1), nextLevel(10 + static_cast<uint32_t>(pow(level, 2))), exp(0),
        hasAbility(false), raceAbilityReady(false), classAbilityReady(false) {
            if (Race == DRAKONIAN) {
                hasAbility = true;
                raceAbilityReady = true;
            }
            if (Class == FIGHTER) {
                hasAbility = true;
                classAbilityReady = true;
            }
            displayStats();
            resources.displayResources();
        }

    void displayStats() const {
        std::cout << "\n-------------------------";
        type ( true,
            "\nName: ", name,
            "\nRace: ", Race.name,
            "\nClass: ", Class.name,
            "\nHealth: ", health, "/", maxhealth,
            "\nStrength: ", STR,
            "\nConstitution: ", CON,
            "\nDefense: ", defense,
            "\nWeapon: ", weapon.name,
            "\nArmor: ", armor.name,
            "\nSpecial: ", special.name,
            "\nLevel: ", level,
            "\nExperience: ", exp, "/", nextLevel
        );
        std::cout << "\n-------------------------\n";
    }

    void receiveGift() {
        resources.addResource("wood", randint(1, 3))
        ->addResource("stone", randint(1, 3))
        ->addResource("fiber", randint(1, 3))
        ->addResource("leather", randint(1, 3));
        exp += randint(5, 15);
    }

    void initArmor(const Item::Armor armor, const std::string source) {
        armor.displayInfo(source);
        equipArmor(armor);
    }

    void initArmor(const Item::Armor::TYPE aType, const std::string source, const uint16_t add = 1) {
        Item::Armor armor(aType, level, add);
        initArmor(armor, source);
    }

    void initWeapon(const Item::Weapon weapon, const std::string source) {
        weapon.displayInfo(source);
        equipWeapon(weapon);
    }

    void initWeapon(const Item::Weapon::TYPE wType, const std::string source, const uint16_t add = 1) {
        Item::Weapon weapon(wType, level, add);
        initWeapon(weapon, source);
    }

    void initSpecial(const Item::Special special, const std::string source) {
        special.displayInfo(source);
        equipSpecial(special);
    }

    void initSpecial(const Item::Special::TYPE sType, const std::string source) {
        Item::Special special(sType, level);
        initSpecial(special, source);
    }

    void equipArmor(const Item::Armor armorItem) {
        if (Race == DRAKONIAN && armorItem != Item::Armor::DRAKONIAN) {
            type("Your unique body shape renders you unable to wear this armor.\n");
            return;
        }
        if (armor != Item::Armor::NONE)
            type("Your current ", armor.name, " has (Defense Bonus: ", armor.defenseBonus, ")\n");

        type("\nDo you want to equip the ", armorItem.name, "? ");
        Choice choice;
        do choice = input("(1. Yes / 2. No): ");
        while (!choice.isChoice(true, "yes", 1, "no", 2));

        if (choice.isChoice("yes", 1)) {
            type("You equip the ", armorItem.name, ".\n");
            defense -= armor.defenseBonus;
            defense += armorItem.defenseBonus;
            armor = armorItem;
        } else
            type("You choose to leave the ", armorItem.name, ".\n");
    }

    void equipWeapon(const Item::Weapon weaponItem) {
        if (weapon != Item::Weapon::NONE)
            type("Your current ", weapon.name, " has (Attack Bonus: ", weapon.attackBonus, ")\n");

        type("\nDo you want to equip the ", weaponItem.name, "? ");
        Choice choice;
        do choice = input("(1. Yes / 2. No): ");
        while (!choice.isChoice(true, "yes", 1, "no", 2));
            
        if (choice.isChoice("yes", 1)) {
            type("You equip the ", weaponItem.name, ".\n");
            STR -= weapon.attackBonus;
            STR += weaponItem.attackBonus;
            weapon = weaponItem;
        } else
            type("You choose to leave the ", weaponItem.name, ".\n");
    }

    void equipSpecial(const Item::Special specialItem) {
        if (special != Item::Special::NONE)
            type("A brittle snap splits the air as your old ", special.name, " crumbles to dust.\n");
        defense -= special.defenseBonus;
        STR -= special.attackBonus;
        defense += specialItem.defenseBonus;
        STR += special.attackBonus;
        special = specialItem;
    }

    void gatherResources() {
        type("Collecting resources...\n");
        wheel();

        Resources colResources;
        colResources.addResource("wood", randint(0, level))
        ->addResource("stone", randint(0, level))
        ->addResource("fiber", randint(0, level))
        ->addResource("leather", randint(0, level));

        resources.addResources(colResources);

        type("Done! Collected resources:\n");
        colResources.displayResources(false);
    }

    const std::string getComponents() const { return ""; }

    template<typename... Args>
    const std::string getComponents(const std::string& name, const uint16_t& amt, const Args&... args) {
        return "\n\t" + ((resources.resources.count(toLower(name)) != 0) ? (name + ": " + std::to_string(resources[toLower(name)]) + '/' + std::to_string(amt)) : "??") + getComponents(args...);
    }

    void craft() {
        Item::Armor drakoArmor(Item::Armor::DRAKONIAN, level, 1, true);
        Item::Armor leatherArmor(Item::Armor::LEATHER, level, 1, true);
        const bool drak = (Race == DRAKONIAN);
        type("What do you want to craft?");
        if (drak) {
            type (
                "\n1. Drakonian Armor (Defense Bonus: " + std::to_string(drakoArmor.defenseBonus) + ", Attack Bonus: 0)"
                "\nComponents:",
                getComponents("Fiber", 2, "Leather", 6)
            );
        } else {
            type (
                "\n1. Leather Armor (Defense Bonus: " + std::to_string(leatherArmor.defenseBonus) + ", Attack Bonus: 0)"
                "\nComponents:",
                getComponents("Fiber", 2, "Leather", 6)
            );
        }
        Item::Weapon longsword(Item::Weapon::LONG, level, 1, true);
        type (
            "\n2. Longsword (Defense Bonus: 0, Attack Bonus: " + std::to_string(longsword.attackBonus) + ")"
            "\nComponents:",
            getComponents("Iron", 5, "Wood", 2, "Fiber", 2),
            "\n3. (go back)\n"
        );
        while (true) {
            Choice choice;
            bool drakChoice;
            bool normChoice;
            bool univChoice;
            do {
                choice = input("Enter choice: ");
                drakChoice = (choice.isChoice("drakonian armor", 1));
                normChoice = (choice.isChoice("leather armor", 1));
                univChoice = (choice.isChoice("longsword", 2));
            } while (!((drak && drakChoice) ||
                    (!drak && normChoice) ||
                    univChoice ||
                    choice.isChoice(true, "go back", "(go back)", 3)));

            if (drak && drakChoice) {
                if (resources["fiber"] < 2 || resources["leather"] < 6) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Drakonian Armor...\n");
                wheel();
                resources["fiber"] -= 2;
                resources["leather"] -= 6;
                initArmor(drakoArmor, "craft");
            } else if (!drak && normChoice) {
                if (resources["fiber"] < 2 || resources["leather"] < 6) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Leather Armor...\n");
                wheel();
                resources["fiber"] -= 2;
                resources["leather"] -= 6;
                initArmor(leatherArmor, "craft");
            } else if (univChoice) {
                if (resources["iron"] < 5 || resources["wood"] < 2 || resources["fiber"] < 2) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Longsword...\n");
                wheel();
                resources["iron"] -= 5;
                resources["wood"] -= 2;
                resources["fiber"] -= 2;
                initWeapon(longsword, "craft");
            }
            break;
        }
    }
    
};