#pragma once
#include"Resources.h"
#include"Enemy.h"
#include"Item.h"

//import Util;

/*class Player {
public:
    std::string name;
    uint16_t maxHealth,
             health,
             maxMana,
             mana,
             baseStrength,
             CON,
             level;
    short strength, defense, 
          mageArmorDefense;
    uint32_t nextLevel, exp;
    Item::Weapon weapon;
    Item::Armor armor;
    Item::Special special;
    bool hasAbility,
         raceAbilityReady,
         classAbilityReady,
         arcaneEye;
    Resources resources;

    enum RACE {
        ELF,
        HUMAN,
        DRAKONIAN,
        REVENANT,
        RCOUNT
    };

    enum CLASS {
        FIGHTER,
        ROGUE,
        WIZARD,
        CCOUNT
    };

    void receiveGift() {
        resources.addResource("wood", randint(1, 3))
        ->addResource("stone", randint(1, 3))
        ->addResource("fiber", randint(1, 3))
        ->addResource("leather", randint(1, 3));
        exp += randint(5, 15);
    }

    const bool rituals() {
        if (!classAbilityReady) {
            type("\nYou cannot perform any more rituals today.\n");
            return 0;
        }
        std::unordered_map<uint16_t, std::string> rituals;
        uint16_t ritualNum = 0;
        rituals[ritualNum++] = "Mana Restoration";
        type("\t", ritualNum, ". Mana Restoration\n");
        type(++ritualNum, ". (go back)\n");
        Choice ritualChoice;
        uint16_t ritualChoiceNum = ritualNum;
        bool isValidRitualChoice = false;
        do {
            ritualChoice = input("Enter choice: ");
            for (uint16_t i = 0; i < ritualNum && !isValidRitualChoice; i++) {
                isValidRitualChoice = ritualChoice.isChoice(rituals[i], i + 1);
                if (isValidRitualChoice)
                    ritualChoiceNum = i;
            }
        } while (!(isValidRitualChoice || ritualChoice.isChoice(true, "(go back)", ritualNum)));
        
        if (rituals[ritualChoiceNum] == "Mana Restoration") {
            if (resources["Arcane Focus"] < 1) {
                type("\nThis ritual requires an arcane focus.\n");
                return 0;
            }
            resources["Arcane Focus"]--;
            mana = std::min((uint16_t)(mana + 4), maxMana);
            type (
                "\nYou channel the energy of your Arcane Focus."
                "\nAs it dissolves, its energy seeps into you, leaving you refreshed and ready to cast once more.\n"
            );
            classAbilityReady = false;
        }
        return 1;
    }

    const bool abilities(Enemy::Enemy* enemy = nullptr, short& mirrorImage = 0) {
        if (!hasAbility && Class != Player::WIZARD) {
            type("\nYou don't have any abilities.\n");
            return 0;
        }
        if (!raceAbilityReady && !classAbilityReady && mana == 0) {
            type("\nYour abilities are currently unavailable.\nYou must sleep first.\n");
            return 0;
        }
        type("\nAbilities:\n");
        std::unordered_map<uint16_t, std::string> abilities;
        uint16_t abilityNum = 0;
        if (enemy != nullptr && Race == Player::DRAKONIAN && raceAbilityReady) {
            abilities[abilityNum++] = "Dragon's Breath";
            type("\t", abilityNum, ". Dragon's Breath (Drakonian)\n");
        }
        if (Class == Player::FIGHTER && classAbilityReady) {
            abilities[abilityNum++] = "Second Wind";
            type("\t", abilityNum, ". Second Wind (Fighter)\n");
        }
        if (Class == Player::WIZARD) {
            type("Spells (You have ", mana, " mana points):\n");
            if (enemy != nullptr) {
                abilities[abilityNum++] = "Fire Bolt";
                type("\t", abilityNum, ". Fire Bolt (Wizard) - 1 MP\n");
                abilities[abilityNum++] = "Mirror Image";
                type("\t", abilityNum, ". Mirror Image (Wizard) - 2 MP\n");
            }
            abilities[abilityNum++] = "Mage Armor";
            type("\t", abilityNum, ". Mage Armor (Wizard) - 5 MP\n");
            if (enemy == nullptr) {
                abilities[abilityNum++] = "Arcane Eye";
                type("\t", abilityNum, ". Arcane Eye (Wizard) - 3 MP\n");
            }
            if (weapon == Item::TYPE::WPN_ST_GUARDIAN) {
                abilities[abilityNum++] = "Recovery";
                type("\t", abilityNum, ". Recovery (Staff of the Guardian) - 2 MP\n");
            }
        }
        type(++abilityNum, ". (go back)\n");
        Choice abilityChoice;
        uint16_t choiceNum = abilityNum;
        bool isValidChoice = false;
        do {
            abilityChoice = input("Enter choice: ");
            for (uint16_t i = 0; i < abilityNum && !isValidChoice; i++) {
                isValidChoice = abilityChoice.isChoice(abilities[i], i + 1);
                if (isValidChoice)
                    choiceNum = i;
            }
        } while (!(isValidChoice || abilityChoice.isChoice(true, "(go back)", abilityNum)));

        if (abilities[choiceNum] == "Dragon's Breath") {
            const uint16_t damage = strength + randint(1, 6);
            const uint16_t burn = randint(1, 4);
            (*enemy).health = std::max((*enemy).health - damage - burn, 0);
            type (
                "\nYou take a deep breath, and with a powerful exhale, a torrent of searing flames erupts from your mouth, searing the ", (*enemy).name, " for ", damage, " damage!"
                "\nThe ", (*enemy).name, " is burned for an additional ", burn, " damage!"
                "\nIts health is now ", (*enemy).health, ".\n"
            );
            raceAbilityReady = false;
        } else if (abilities[choiceNum] == "Second Wind") {
            const uint16_t healing = heal();
            type (
                "\nYou get a surge of adrenaline and heal ", healing, " points!"
                "\nYour health is now ", health, ".\n"
            );
            classAbilityReady = false;
        } else if (abilities[choiceNum] == "Fire Bolt") {
            if (mana < 1) {
                type("\nYou don't have enough mana points!\n");
                return 0;
            }
            const uint16_t damage = strength + randint(1, 8);
            const uint16_t burn = randint(1, 4);
            (*enemy).health = std::max((*enemy).health - damage - burn, 0);
            mana--;
            health = std::min((uint16_t)(health + 1), maxHealth);
            type (
                "\nYou conjure a blazing ember in your palm and hurl it forward."
                "\nThe fire bolt streaks through the air, striking the ", (*enemy).name, " with a burst of flames for ", damage, " damage!"
                "\nThe ", (*enemy).name, " is burned for an additional ", burn, " damage!"
                "\nIts health is now ", (*enemy).health, ".\n"
            );
        } else if (abilities[choiceNum] == "Mirror Image") {
            if (mana < 2) {
                type("\nYou don't have enough mana points!\n");
                return 0;
            }
            mirrorImage = 2;
            mana -= 2;
            health = std::min((uint16_t)(health + 2), maxHealth);
            type (
                "\nYou weave an illusion, creating shimmering duplicates of yourself."
                "\nThey flicker and shift, making it difficult for the enemy to land its strikes.\n"
            );
        } else if (abilities[choiceNum] == "Mage Armor") {
            if (mageArmorDefense > 0) {
                type("\nThis spell is already active!\n");
                return 0;
            }
            if (mana < 5) {
                type("\nYou don't have enough mana points!\n");
                return 0;
            }
            mageArmorDefense = pow(level + 1, 1.2f);
            defense += mageArmorDefense;
            mana -= 5;
            health = std::min((uint16_t)(health + 5), maxHealth);
            type ("\nA protective shielding aura surrounds you, boosting your defense by ", mageArmorDefense, "!\n");
        } else if (abilities[choiceNum] == "Arcane Eye") {
            if (mana < 3) {
                type("\nYou don't have enough mana points!\n");
                return 0;
            }
            arcaneEye = true;
            mana -= 3;
            health = std::min((uint16_t)(health + 3), maxHealth);
            type (
                "\nYou materialize your magic, forming an invisible, floating eye."
                "\nIt scouts ahead, ensuring you are never caught off guard.\n"
            );
        } else if (abilities[choiceNum] == "Recovery") {
            if (mana < 2) {
                type("\nYou don't have enough mana points!\n");
                return 0;
            }
            const uint16_t healing = heal(3);
            mana -= 2;
            health = std::min((uint16_t)(health + 2), maxHealth);
            type (
                "\nYou channel magical energy into a healing aura, wrapping yourself in a warm, sooting light."
                "\nYour wounds are mended and you heal ", healing, " points!"
                "\nYour health is now ", health, ".\n"
            );
        }
        return 1;
    }

    const uint16_t heal(const uint16_t div = 2) {
        static const float HEALING_MULTIPLIER = randint(10, 15) / 10.f;
        const uint16_t healing = (maxHealth - health) * HEALING_MULTIPLIER / div;
        health += healing;
        return healing;
    }

    void initItem(const Item::Item& item, const Item::Source source) {
        item.displayInfo(source);
        resources.addResource(item.name);
    }

    void initItem(const Item::TYPE iType, const Item::Source source) {
        Item::Item item(iType);
        initItem(item, source);
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

    void unequipArmor(const bool confirmation = true) {
        if (confirmation) {
            Choice choice;
            do choice = input("Are you sure you want to unequip your ", armor.name, " (1. Yes / 2. No)? ");
            while (!choice.isChoice(true, "yes", 1, "no", 2));
            if (choice.isChoice("no", 2))
                return;
            type("You unequip your ", armor.name, ".\n");
        }
        defense -= armor.defenseBonus;
        armor = Item::Armor();
    }

    void unequipWeapon(const bool confirmation = true) {
        if (confirmation) {
            Choice choice;
            do choice = input("Are you sure you want to unequip your ", weapon.name, " (1. Yes / 2. No)? ");
            while (!choice.isChoice(true, "yes", 1, "no", 2));
            if (choice.isChoice("no", 2))
                return;
            type("You unequip your ", weapon.name, ".\n");
        }
        if (weapon == Item::TYPE::WPN_ST_WARBORN) {
            maxMana -= 3;
            mana = std::max(mana - 3, 0);
        } else if (weapon == Item::TYPE::WPN_ST_WEEPING) {
            maxMana += 3;
            mana += 3;
        }
        strength -= weapon.strengthBonus;
        weapon = Item::Weapon();
    }

    void gatherResources() {
        type("\nCollecting resources...\n");
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

    void craft() {
        do {
            std::unordered_map<uint16_t, std::unique_ptr<Item::Item>> items;
            uint16_t i = 0;
            type (
                "\nWhat do you want to craft?"
                "\n1. Armor"
                "\n2. Weapons"
                "\n3. Items"
                "\n4. (go back)\n"
            );
            Choice typeChoice;
            do typeChoice = input("Enter choice: ");
            while (!typeChoice.isChoice(true, "armor", 1, "weapons", 2, "items", 3, "(go back)", "go back", 4));

            if (typeChoice.isChoice("(go back)", "go back", 4))
                return;

            std::cout << '\n';

            if (typeChoice.isChoice("armor", 1))
                if (!initCraftArmor(items, i))
                    continue;
            else if (typeChoice.isChoice("weapons", 2))
                if (!initCraftWeapons(items, i)) [[unlikely]]
                    continue;
            else if (typeChoice.isChoice("items", 3))
                if (!initCraftItems(items, i))
                    continue;
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

                switch ((*items[choiceNum]).itemType) {
                case Item::TYPE::ARM_DRAKONIAN: // Drakonian Armor
                craftArmor(static_cast<Item::Armor&>(*items[choiceNum]), "fiber", 2, "leather", 6);
                case Item::TYPE::ARM_LEATHER: // Leather Armor
                    craftArmor(static_cast<Item::Armor&>(*items[choiceNum]), "fiber", 2, "leather", 6);
                case Item::TYPE::WPN_LONG: // Longsword
                    craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "fiber", 2, "iron", 3, "wood", 2);
                case Item::TYPE::WPN_ST_WARBORN: // Staff of the Warborn
                    craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Warborn", 1, "fiber", 2, "wood", 4);
                case Item::TYPE::WPN_ST_GUARDIAN: // Staff of the Guardian
                    craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Guardian", 1, "fiber", 2, "wood", 4);
                case Item::TYPE::WPN_ST_SHADOW: // Staff of the Shadow
                    craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Shadow", 1, "fiber", 2, "wood", 4);
                case Item::TYPE::WPN_ST_FURY: // Staff of Fury
                    craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of Fury", 1, "fiber", 2, "wood", 4);
                case Item::TYPE::WPN_ST_WEEPING: // Staff of the Weeping Spirit
                    craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Weeping Spirit", 1, "fiber", 2, "wood", 4);
                case Item::TYPE::FOCUS: // Arcane Focus
                    craftItem(*items[choiceNum], "crystals", 4);
                }
                return;
            }
        } while (true);
    }
    
private:

    struct RaceInfo {
        const RACE Race;
        const std::string name;
        const bool operator==(const RACE compRace) const {
            return Race == compRace;
        }
        const bool operator!=(const RACE compRace) const {
            return Race != compRace;
        }
    };
    
    inline static const RaceInfo R[RACE::RCOUNT] = {
        { ELF,       "Elf"       },
        { HUMAN,     "Human"     },
        { DRAKONIAN, "Drakonian" },
        { REVENANT,  "Revenant"  }
    };
    
    struct ClassInfo {
        const CLASS Class;
        const std::string name;
        const bool operator==(const CLASS compClass) const {
            return Class == compClass;
        }
        const bool operator!=(const CLASS compClass) const {
            return Class != compClass;
        }
    };
    
    inline static const ClassInfo C[CLASS::CCOUNT] = {
        { FIGHTER, "Fighter" },
        { ROGUE,   "Rogue"   },
        { WIZARD,  "Wizard"  }
    };

    const bool initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) {
        if (Class == WIZARD) {
            type("There is no armor available to craft.\n");
            return 0;
        }
        if (Race == DRAKONIAN) {
            Item::Armor drakArmor(Item::TYPE::ARM_DRAKONIAN, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Armor>(drakArmor));
            type (
                ++i, ". Drakonian Armor (Defense Bonus: ", drakArmor.defenseBonus, ")",
                displayComponents("fiber", 2, "leather", 6), "\n"
            );
        } else {
            Item::Armor leatherArmor(Item::TYPE::ARM_LEATHER, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Armor>(leatherArmor));
            type (
                ++i, ". Leather Armor (Defense Bonus: ", leatherArmor.defenseBonus, ")",
                displayComponents("fiber", 2, "leather", 6), "\n"
            );
        }
        return 1;
    }

    const bool initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) {
        Item::Weapon longsword(Item::TYPE::WPN_LONG, level, (Class == WIZARD ? -1 : 1), true);
        items[i] = std::move(std::make_unique<Item::Weapon>(longsword));
        type (
            ++i, ". Longsword (Strength Bonus: ", longsword.strengthBonus, ")",
            displayComponents("fiber", 2, "iron", 3, "wood", 2), "\n"
        );
        if (Class == WIZARD) {
            Item::Weapon stWarborn(Item::TYPE::WPN_ST_WARBORN, level, (Race == ELF ? 2 : 1), true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stWarborn));
            type (
                ++i, ". Staff of the Warborn (Strength Bonus: ", stWarborn.strengthBonus, ")",
                displayComponents("Amulet of the Warborn", 1, "fiber", 2, "wood", 4), "\n"
            );
            Item::Weapon stGuardian(Item::TYPE::WPN_ST_GUARDIAN, level, (Race == ELF ? 2 : 1), true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stGuardian));
            type (
                ++i, ". Staff of the Guardian (Strength Bonus: ", stGuardian.strengthBonus, ")",
                displayComponents("Amulet of the Guardian", 1, "fiber", 2, "wood", 4), "\n"
            );
            Item::Weapon stShadow(Item::TYPE::WPN_ST_SHADOW, level, 1, true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stShadow));
            type (
                ++i, ". Staff of the Shadow (Strength Bonus: ", stShadow.strengthBonus, ")",
                displayComponents("Amulet of the Shadow", 1, "fiber", 2, "wood", 4), "\n"
            );
            Item::Weapon stFury(Item::TYPE::WPN_ST_FURY, level, (Race == ELF ? 2 : 1), true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stFury));
            type (
                ++i, ". Staff of Fury (Strength Bonus: ", stFury.strengthBonus, ")",
                displayComponents("Amulet of Fury", 1, "fiber", 2, "wood", 4), "\n"
            );
            Item::Weapon stWeeping(Item::TYPE::WPN_ST_WEEPING, level, (Race == ELF ? 2 : 1), true);
            items[i] = std::move(std::make_unique<Item::Weapon>(stWeeping));
            type (
                ++i, ". Staff of the Weeping Spirit (Strength Bonus: ", stWeeping.strengthBonus, ")",
                displayComponents("Amulet of the Weeping Spirit", 1, "fiber", 2, "wood", 4), "\n"
            );
        }
        return 1;
    }

    const bool initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) {
        if (Class != WIZARD) {
            type("There are no items available to craft.\n");
            return 0;
        }
        Item::Item focus(Item::TYPE::FOCUS);
        items[i] = std::move(std::make_unique<Item::Item>(focus));
        type (
            ++i, ". Arcane Focus",
            displayComponents("crystals", 4), "\n"
        );
        return 1;
    }

    const bool checkComponents() const {
        return true;
    }

    template<typename... Args>
    const bool checkComponents(const std::string& resource, const uint16_t amount, const Args&... args) {
        return resources[resource] >= amount && checkComponents(args...);
    }

    void useComponents() {}
    
    template<typename... Args>
    void useComponents(const std::string& resource, const uint16_t amount, const Args&... args) {
        resources[resource] -= amount;
        useComponents(args...);
    }
    
    template<typename... Args>
    const bool initCraft(const Item::Item* const& item, const Args&... args) {
        if (!checkComponents(args...)) {
            type("You don't have enough resources!\n");
            return 0;
        }
        useComponents(args...);
        type("\nCrafting ", (*item).name, "...\n");
        wheel();
        return 1;
    }

    template<typename... Args>
    const bool craftArmor(Item::Armor& armor, const Args&... args) {
        if (!initCraft(&armor, args...))
            return 0;
        initArmor(armor, Item::Source::CRAFT);
        return 1;
    }

    template<typename... Args>
    const bool craftWeapon(const Item::Weapon& weapon, const Args&... args) {
        if (!initCraft(&weapon, args...))
            return 0;
        initWeapon(weapon, Item::Source::CRAFT);
        return 1;
    }

    template<typename... Args>
    const bool craftItem(const Item::Item& item, const Args&... args) {
        if (!initCraft(&item, args...))
            return 0;
        initItem(item, Item::Source::CRAFT);
        return 1;
    }

    void equipArmor(const Item::Armor& armorItem) {
        if (Race == DRAKONIAN && armorItem != Item::TYPE::ARM_DRAKONIAN) {
            type("Your unique body shape renders you unable to wear this armor.\n");
            return;
        }
        if (Class == WIZARD) {
            type("The weight and rigidity of this armor would interfere with your connection to the arcane, preventing you from casting spells while wearing it.\n");
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
            unequipArmor(false);
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
            unequipWeapon(false);
            strength += weaponItem.strengthBonus;
            weapon = weaponItem;
            if (weapon == Item::TYPE::WPN_ST_WARBORN) {
                maxMana += 3;
                mana += 3;
            } else if (weapon == Item::TYPE::WPN_ST_WEEPING) {
                maxMana -= 3;
                mana = std::max(mana - 3, 0);
            }
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
    
    const std::string getComponents() const { return ""; }
    
    template<typename... Args>
    const std::string getComponents(const std::string& name, const uint16_t& amt, const Args&... args) {
        return "\n\t" + ((resources.resources.count(name) != 0) ? (name + ": " + std::to_string(resources[name]) + '/' + std::to_string(amt)) : "??") + getComponents(args...);
    }
    
    template<typename... Args>
    const std::string displayComponents(const Args&... args) {
        return "\nComponents:" + getComponents(args...);
    }

public:

    RaceInfo Race;
    ClassInfo Class;

    Player(std::string n, RACE ra, CLASS cl, int h, int s, int co) :
      name(n), Race({R[ra].Race, R[ra].name}), Class({C[cl].Class, C[cl].name}), maxHealth(h), health(h),
      maxMana(0), mana(0), baseStrength(s), strength(s), CON(co), defense(0), mageArmorDefense(0),
      level(1), nextLevel(10 + static_cast<uint32_t>(pow(level, 2))), exp(0), hasAbility(false),
      raceAbilityReady(false), classAbilityReady(false), arcaneEye(false) {
        if (Race == DRAKONIAN) {
            hasAbility = true;
            raceAbilityReady = true;
        }
        if (Class == FIGHTER) {
            hasAbility = true;
            classAbilityReady = true;
        } else if (Class == WIZARD) {
            hasAbility = true;
            classAbilityReady = true;
            maxMana = 15;
            mana = 15;
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
            (Class == WIZARD ? "\nMana Points: " + std::to_string(mana) + '/' + std::to_string(maxMana) : ""),
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

};*/

class PlayerPublic;
class PlayerPrivate;

class Player {
public:
    std::string name;
    uint16_t maxHealth,
             health,
             maxMana,
             mana,
             baseStrength,
             CON,
             level;
    short strength, defense, 
    mageArmorDefense;
    uint32_t nextLevel, exp;
    Item::Weapon weapon;
    Item::Armor armor;
    Item::Special special;
    bool hasAbility,
         raceAbilityReady,
         classAbilityReady,
         arcaneEye;
    Resources resources;

    enum RACE {
        ELF,
        HUMAN,
        DRAKONIAN,
        REVENANT
    };
    
    enum CLASS {
        FIGHTER,
        ROGUE,
        WIZARD
    };
 
    void receiveGift();
    const bool rituals();
    const bool abilities(Enemy::Enemy* enemy = nullptr, short* mirrorImage = nullptr);
    const uint16_t heal(const uint16_t div = 2);
    void initItem(const Item::Item& item, const Item::Source source);
    void initItem(const Item::TYPE iType, const Item::Source source);
    void initArmor(const Item::Armor& armor, const Item::Source source);
    void initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add = 1);
    void initWeapon(const Item::Weapon& weapon, const Item::Source source);
    void initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add = 1);
    void initSpecial(const Item::Special& special, const Item::Source source);
    void initSpecial(const Item::TYPE sType, const Item::Source source);
    void unequipArmor(const bool confirmation = true);
    void unequipWeapon(const bool confirmation = true);
    void gatherResources();
    void craft();
    
private:
    std::unique_ptr<PlayerPublic>  pPub;
    std::unique_ptr<PlayerPrivate> pPrv;
    
    struct RaceInfo {
        const RACE Race;
        const std::string_view name;
        const bool operator==(const RACE compRace) const {
            return Race == compRace;
        }
        const bool operator!=(const RACE compRace) const {
            return Race != compRace;
        }
    };

    inline static constexpr RaceInfo R[] {
        { ELF,       "Elf"       },
        { HUMAN,     "Human"     },
        { DRAKONIAN, "Drakonian" },
        { REVENANT,  "Revenant"  }
    };
    
    struct ClassInfo {
        const CLASS Class;
        const std::string_view name;
        const bool operator==(const CLASS compClass) const {
            return Class == compClass;
        }
        const bool operator!=(const CLASS compClass) const {
            return Class != compClass;
        }
    };
    
    inline static constexpr ClassInfo C[] {
        { FIGHTER, "Fighter" },
        { ROGUE,   "Rogue"   },
        { WIZARD,  "Wizard"  }
    };

    const bool initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i);
    const bool initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i);
    const bool initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i);
    const bool checkComponents() const;
    template<typename... Args> const bool checkComponents(const std::string& resource, const uint16_t amount, const Args&... args);
    void useComponents();
    template<typename... Args> void useComponents(const std::string& resource, const uint16_t amount, const Args&... args);
    template<typename... Args> const bool initCraft(const Item::Item* const& item, const Args&... args);
    template<typename... Args> const bool craftArmor(Item::Armor& armor, const Args&... args);
    template<typename... Args> const bool craftWeapon(const Item::Weapon& weapon, const Args&... args);
    template<typename... Args> const bool craftItem(const Item::Item& item, const Args&... args);
    void equipArmor(const Item::Armor& armorItem);
    void equipWeapon(const Item::Weapon& weaponItem);
    void equipSpecial(const Item::Special& specialItem);
    const std::string getComponents() const;
    template<typename... Args> const std::string getComponents(const std::string& name, const uint16_t& amt, const Args&... args);
    template<typename... Args> const std::string displayComponents(const Args&... args);

public:

    RaceInfo Race;
    ClassInfo Class;

    Player(std::string, RACE, CLASS, int, int, int);
    ~Player();

    void displayStats() const;

};