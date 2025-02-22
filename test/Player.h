#pragma once
#include<string>
#include<cmath>
#include"Resources.h"
#include"Item.h"
//import Util;

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
        WIZARD,
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
            { ROGUE,   "Rogue"   },
            { WIZARD,  "Wizard"  }
    };

    std::string name;
    RA Race;
    CL Class;
    uint16_t maxHealth, health, baseStrength, CON, level;
    short strength, defense;
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
        maxHealth(h), health(h), baseStrength(s), strength(s), CON(co), defense(0),
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
            "\nHealth: ", health, "/", maxHealth,
            "\nStrength: ", strength,
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

    void initArmor(const Item::Armor& armor, const Item::Source source) {
        armor.displayInfo(source);
        equipArmor(armor);
    }

    void initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add = 1) {
        Item::Armor armor(aType, level, add);
        initArmor(armor, source);
    }

    void initWeapon(const Item::Weapon& weapon, const Item::Source source) {
        weapon.displayInfo(source);
        equipWeapon(weapon);
    }

    void initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add = 1) {
        Item::Weapon weapon(wType, level, add);
        initWeapon(weapon, source);
    }

    void initSpecial(const Item::Special& special, const Item::Source source) {
        special.displayInfo(source);
        equipSpecial(special);
    }

    void initSpecial(const Item::TYPE sType, const Item::Source source) {
        Item::Special special(sType, level);
        initSpecial(special, source);
    }

    void equipArmor(const Item::Armor& armorItem) {
        if (Race == DRAKONIAN && armorItem != Item::TYPE::ARM_DRAKONIAN) {
            type("Your unique body shape renders you unable to wear this armor.\n");
            return;
        }
        if (armor != Item::TYPE::NONE)
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

    void equipWeapon(const Item::Weapon& weaponItem) {
        if (weapon != Item::TYPE::NONE)
            type("Your current ", weapon.name, " has (Strength Bonus: ", weapon.strengthBonus, ")\n");

        type("\nDo you want to equip the ", weaponItem.name, "? ");
        Choice choice;
        do choice = input("(1. Yes / 2. No): ");
        while (!choice.isChoice(true, "yes", 1, "no", 2));
            
        if (choice.isChoice("yes", 1)) {
            type("You equip the ", weaponItem.name, ".\n");
            strength -= weapon.strengthBonus;
            strength += weaponItem.strengthBonus;
            weapon = weaponItem;
        } else
            type("You choose to leave the ", weaponItem.name, ".\n");
    }

    void equipSpecial(const Item::Special& specialItem) {
        if (special != Item::TYPE::NONE) {
            type("A brittle snap splits the air as your old ", special.name, " crumbles to dust.\n");
            resources[special.name] = 0;
        }
        defense -= special.defenseBonus;
        strength -= special.strengthBonus;
        defense += specialItem.defenseBonus;
        strength += special.strengthBonus;
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
        return "\n\t" + ((resources.resources.count(name) != 0) ? (name + ": " + std::to_string(resources[name]) + '/' + std::to_string(amt)) : "??") + getComponents(args...);
    }

    void craft() {
        std::unordered_map<uint16_t, std::unique_ptr<Item::Item>> items;
        uint16_t i = 0;
        const bool drak = (Race == DRAKONIAN);
        const bool wiz = (Class == WIZARD);
        type("What do you want to craft?\n");
        if (drak) {
            Item::Armor drakArmor(Item::TYPE::ARM_DRAKONIAN, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Armor>(drakArmor));
            type (
                ++i, ". Drakonian Armor (Defense Bonus: ", drakArmor.defenseBonus, ")"
                "\nComponents:",
                getComponents("leather", 6, "fiber", 2), "\n"
            );
        } else {
            Item::Armor leatherArmor(Item::TYPE::ARM_LEATHER, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Armor>(leatherArmor));
            type (
                ++i, ". Leather Armor (Defense Bonus: ", leatherArmor.defenseBonus, ")"
                "\nComponents:",
                getComponents("leather", 6, "fiber", 2), "\n"
            );
        } {
            Item::Weapon longsword(Item::TYPE::WPN_LONG, level, (wiz ? -1 : 1), true);
            items[i] = std::move(std::make_unique<Item::Weapon>(longsword));
            type (
                ++i, ". Longsword (Strength Bonus: ", longsword.strengthBonus, ")"
                "\nComponents:",
                getComponents("wood", 2, "fiber", 2, "Iron", 3), "\n"
            );
        } if (wiz) {
            Item::Weapon stWarborn(Item::TYPE::WPN_ST_WARBORN, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stWarborn));
            type (
                ++i, ". Staff of the Warborn (Strength Bonus: ", stWarborn.strengthBonus, ")"
                "\nComponents:",
                getComponents("wood", 4, "fiber", 2, "Amulet of the Warborn", 1), "\n"
            );
            Item::Weapon stGuardian(Item::TYPE::WPN_ST_GUARDIAN, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stGuardian));
            type (
                ++i, ". Staff of the Guardian (Strength Bonus: ", stGuardian.strengthBonus, ")"
                "\nComponents:",
                getComponents("wood", 4, "fiber", 2, "Amulet of the Guardian", 1), "\n"
            );
            Item::Weapon stShadow(Item::TYPE::WPN_ST_SHADOW, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stShadow));
            type (
                ++i, ". Staff of the Shadow (Strength Bonus: ", stShadow.strengthBonus, ")"
                "\nComponents:",
                getComponents("wood", 4, "fiber", 2, "Amulet of the Shadow", 1), "\n"
            );
            Item::Weapon stFury(Item::TYPE::WPN_ST_FURY, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stFury));
            type (
                ++i, ". Staff of Fury (Strength Bonus: ", stFury.strengthBonus, ")"
                "\nComponents:",
                getComponents("wood", 4, "fiber", 2, "Amulet of Fury", 1), "\n"
            );
            Item::Weapon stWeeping(Item::TYPE::WPN_ST_WEEPING, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stWeeping));
            type (
                ++i, ". Staff of the Weeping Spirit (Strength Bonus: ", stWeeping.strengthBonus, ")"
                "\nComponents:",
                getComponents("wood", 4, "fiber", 2, "Amulet of the Weeping Spirit", 1), "\n"
            );
        }
        type(i + 1, ". (go back)\n");

        while (true) {
            Choice craftChoice;
            uint16_t choiceNum = i;
            bool isValidChoice = false;
            do {
                craftChoice = input("Enter choice: ");
                for (uint16_t j = 0; j < i && !isValidChoice; j++) {
                    isValidChoice = craftChoice.isChoice(items[j]->name, j + 1);
                    if (isValidChoice)
                        choiceNum = j;
                }
            } while (!(isValidChoice || craftChoice.isChoice(true, "(go back)", i + 1)));

            if (choiceNum == i)
                break;

            const bool drakChoice = (*items[choiceNum] == Item::TYPE::ARM_DRAKONIAN);
            const bool normChoice = (*items[choiceNum] == Item::TYPE::ARM_LEATHER);
            const bool univChoice = (*items[choiceNum] == Item::TYPE::WPN_LONG);
            if (drakChoice) { // Drakonian Armor
                if (resources["fiber"] < 2 || resources["leather"] < 6) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Drakonian Armor...\n");
                wheel();
                resources["leather"] -= 6;
                resources["fiber"] -= 2;
                initArmor(static_cast<Item::Armor&>(*items[choiceNum]), Item::Source::CRAFT);
            } else if (normChoice) { // Leather Armor
                if (resources["fiber"] < 2 || resources["leather"] < 6) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Leather Armor...\n");
                wheel();
                resources["leather"] -= 6;
                resources["fiber"] -= 2;
                initArmor(static_cast<Item::Armor&>(*items[choiceNum]), Item::Source::CRAFT);
            } else if (univChoice) { // Longsword
                if (resources["fiber"] < 2 || resources["wood"] < 2 || resources["iron"] < 3) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Longsword...\n");
                wheel();
                resources["wood"] -= 2;
                resources["fiber"] -= 2;
                resources["iron"] -= 3;
                initWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), Item::Source::CRAFT);
            } else if (*items[choiceNum] == Item::TYPE::WPN_ST_WARBORN) { // Staff of the Warborn
                if (resources["fiber"] < 2 || resources["wood"] < 4 || resources["Amulet of the Warborn"] < 1) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Staff of the Warborn...\n");
                wheel();
                resources["wood"] -= 4;
                resources["fiber"] -= 2;
                resources["Amulet of the Warborn"] -= 1;
                defense -= special.defenseBonus;
                strength -= special.strengthBonus;
                special = Item::Special();
                initWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), Item::Source::CRAFT);
            } else if (*items[choiceNum] == Item::TYPE::WPN_ST_GUARDIAN) { // Staff of the Guardian
                if (resources["fiber"] < 2 || resources["wood"] < 4 || resources["Amulet of the Guardian"] < 1) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Staff of the Guardian...\n");
                wheel();
                resources["wood"] -= 4;
                resources["fiber"] -= 2;
                resources["Amulet of the Guardian"] -= 1;
                defense -= special.defenseBonus;
                strength -= special.strengthBonus;
                special = Item::Special();
                initWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), Item::Source::CRAFT);
            } else if (*items[choiceNum] == Item::TYPE::WPN_ST_SHADOW) { // Staff of the Shadow
                if (resources["fiber"] < 2 || resources["wood"] < 4 || resources["Amulet of the Shadow"] < 1) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Staff of the Shadow...\n");
                wheel();
                resources["wood"] -= 4;
                resources["fiber"] -= 2;
                resources["Amulet of the Shadow"] -= 1;
                defense -= special.defenseBonus;
                strength -= special.strengthBonus;
                special = Item::Special();
                initWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), Item::Source::CRAFT);
            } else if (*items[choiceNum] == Item::TYPE::WPN_ST_FURY) { // Staff of Fury
                if (resources["fiber"] < 2 || resources["wood"] < 4 || resources["Amulet of Fury"] < 1) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Staff of Fury...\n");
                wheel();
                resources["wood"] -= 4;
                resources["fiber"] -= 2;
                resources["Amulet of Fury"] -= 1;
                defense -= special.defenseBonus;
                strength -= special.strengthBonus;
                special = Item::Special();
                initWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), Item::Source::CRAFT);
            } else if (*items[choiceNum] == Item::TYPE::WPN_ST_WEEPING) { // Staff of the Weeping Spirit
                if (resources["fiber"] < 2 || resources["wood"] < 4 || resources["Amulet of the Weeping Spirit"] < 1) {
                    type("You don't have enough resources!\n");
                    continue;
                }
                type("\nCrafting Staff of the Weeping Spirit...\n");
                wheel();
                resources["wood"] -= 4;
                resources["fiber"] -= 2;
                resources["Amulet of the Weeping Spirit"] -= 1;
                defense -= special.defenseBonus;
                strength -= special.strengthBonus;
                special = Item::Special();
                initWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), Item::Source::CRAFT);
            }
            break;
        }
    }
    
};