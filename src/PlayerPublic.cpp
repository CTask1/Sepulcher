#include<unordered_map>
#include<iostream>
#include<memory>
#include<string>

#include"..\include\PlayerPrivate.h"
#include"..\include\PlayerPublic.h"
#include"..\include\Player.h"
#include"..\include\Enemy.h"
#include"..\include\Util.h"

uint16_t PlayerPublic::getMaxHealth() const {
    uint16_t maxHealth = player.maxHealth;
    for (const Debuff& debuff : player.debuffs) {
        if (debuff.hMod != 0)
            maxHealth *= (1 + debuff.hMod);
    }
    return maxHealth;
}

short PlayerPublic::getStrength() const {
    short strength = player.strength;
    for (const Debuff& debuff : player.debuffs) {
        if (debuff.strMod != 0)
            strength *= (1 + debuff.strMod);
    }
    return strength;
}

short PlayerPublic::getDefense() const {
    short defense = player.defense;
    for (const Debuff& debuff : player.debuffs) {
        if (debuff.defMod != 0)
            defense *= (1 + debuff.defMod);
    }
    return defense;
}

void PlayerPublic::receiveGift() {
    player.resources.addResource("wood", randint(1, 3))
    ->addResource("stone", randint(1, 3))
    ->addResource("fiber", randint(1, 3))
    ->addResource("leather", randint(1, 3));
    player.exp += randint(5, 15);
}

bool PlayerPublic::rituals() {
    if (!player.classAbilityReady) {
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
        if (player.resources["Arcane Focus"] < 1) {
            type("\nThis ritual requires an arcane focus.\n");
            return 0;
        }
        player.resources["Arcane Focus"]--;
        player.mana = std::min((uint16_t)(player.mana + 4), player.maxMana);
        type (
            "\nYou channel the energy of your Arcane Focus."
            "\nAs it dissolves, its energy seeps into you, leaving you refreshed and ready to cast once more.\n"
        );
        player.classAbilityReady = false;
    }
    return 1;
}

bool PlayerPublic::abilities(Enemy::Enemy* enemy, short* mirrorImage, bool* shadowmeld) {
    if (!player.hasAbility && player.Class != Player::WIZARD) {
        type("\nYou don't have any abilities.\n");
        return 0;
    }
    if (!player.raceAbilityReady && !player.classAbilityReady && player.mana == 0) {
        type("\nYour abilities are currently unavailable.\nYou must sleep first.\n");
        return 0;
    }
    type("\nAbilities:\n");
    std::unordered_map<uint16_t, std::string> abilities;
    uint16_t abilityNum = 0;
    // Race abilities
    if (enemy != nullptr && player.Race == Player::DRAKONIAN && player.raceAbilityReady) {
        abilities[abilityNum++] = "Dragon's Breath";
        type("\t", abilityNum, ". Dragon's Breath (Drakonian)\n");
    } else if (enemy != nullptr && player.Race == Player::REVENANT) {
        if (player.raceAbilityReady) {
            abilities[abilityNum++] = "Shadowmeld";
            type("\t", abilityNum, ". Shadowmeld (Revenant)\n");
        }
        if (player.bloodMeter < 3) {
            abilities[abilityNum++] = "Necrotic Drain";
            type("\t", abilityNum, ". Necrotic Drain ", player.bloodMeter, "/3 (Revenant)\n");
        }
    }
    // Class abilities
    if (player.Class == Player::FIGHTER && player.classAbilityReady) {
        abilities[abilityNum++] = "Second Wind";
        type("\t", abilityNum, ". Second Wind (Fighter)\n");
    }
    if (player.Class == Player::WIZARD) {
        type("Spells (You have ", player.mana, " mana points):\n");
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
        if (player.weapon == Item::TYPE::WPN_ST_GUARDIAN) {
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
        const uint16_t damage = player.strength + randint(1, 6);
        const uint16_t burn = randint(1, 4);
        (*enemy).health = std::max((*enemy).health - damage - burn, 0);
        type (
            "\nYou take a deep breath, and with a powerful exhale, a torrent of searing flames erupts from your mouth, searing the ", (*enemy).name, " for ", damage, " damage!"
            "\nThe ", (*enemy).name, " is burned for an additional ", burn, " damage!"
            "\nIts health is now ", (*enemy).health, ".\n"
        );
        player.raceAbilityReady = false;
    } else if (abilities[choiceNum] == "Shadowmeld") {
        type (
            "\nYou meld into the shadows, becoming nearly invisible."
            "\nThe enemy can't see you!\n"
        );
        *shadowmeld = true;
        player.raceAbilityReady = false;
        return 0;
    } else if (abilities[choiceNum] == "Necrotic Drain") {
        const uint16_t damage = player.strength / 2 + randint(1, 6);
        (*enemy).health = std::max((*enemy).health - damage, 0);
        player.health = std::min((uint16_t)(player.health + damage / 2), player.getMaxHealth());
        player.bloodMeter++;
        type (
            "\nYou drain the life force from the ", (*enemy).name, ", dealing ", damage, " damage!"
            "\nIts health is now ", (*enemy).health, "."
            "\nYou also fill your blood meter and gain ", damage / 2, " health points!"
            "\nYour health is now ", player.health, "."
            "\nYour blood meter is now at ", player.bloodMeter, "/3.\n"
        );
    } else if (abilities[choiceNum] == "Second Wind") {
        const uint16_t healing = player.heal();
        type (
            "\nYou get a surge of adrenaline and heal ", healing, " points!"
            "\nYour health is now ", player.health, ".\n"
        );
        player.classAbilityReady = false;
    } else if (abilities[choiceNum] == "Fire Bolt") {
        if (player.mana < 1) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        const uint16_t damage = player.strength + randint(1, 8);
        const uint16_t burn = randint(1, 4);
        (*enemy).health = std::max((*enemy).health - damage - burn, 0);
        player.mana--;
        player.health = std::min((uint16_t)(player.health + 1), player.getMaxHealth());
        type (
            "\nYou conjure a blazing ember in your palm and hurl it forward."
            "\nThe fire bolt streaks through the air, striking the ", (*enemy).name, " with a burst of flames for ", damage, " damage!"
            "\nThe ", (*enemy).name, " is burned for an additional ", burn, " damage!"
            "\nIts health is now ", (*enemy).health, ".\n"
        );
    } else if (abilities[choiceNum] == "Mirror Image") {
        if (player.mana < 2) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        *mirrorImage = 2;
        player.mana -= 2;
        player.health = std::min((uint16_t)(player.health + 2), player.getMaxHealth());
        type (
            "\nYou weave an illusion, creating shimmering duplicates of yourself."
            "\nThey flicker and shift, making it difficult for the enemy to land its strikes.\n"
        );
    } else if (abilities[choiceNum] == "Mage Armor") {
        if (player.mageArmorDefense > 0) {
            type("\nThis spell is already active!\n");
            return 0;
        }
        if (player.mana < 5) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        player.mageArmorDefense = pow(player.level + 1, 1.2f);
        player.defense += player.mageArmorDefense;
        player.mana -= 5;
        player.health = std::min((uint16_t)(player.health + 5), player.getMaxHealth());
        type ("\nA protective shielding aura surrounds you, boosting your defense by ", player.mageArmorDefense, "!\n");
    } else if (abilities[choiceNum] == "Arcane Eye") {
        if (player.mana < 3) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        player.arcaneEye = true;
        player.mana -= 3;
        player.health = std::min((uint16_t)(player.health + 3), player.getMaxHealth());
        type (
            "\nYou materialize your magic, forming an invisible, floating eye."
            "\nIt scouts ahead, ensuring you are never caught off guard.\n"
        );
    } else if (abilities[choiceNum] == "Recovery") {
        if (player.mana < 2) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        const uint16_t healing = player.heal(3);
        player.mana -= 2;
        player.health = std::min((uint16_t)(player.health + 2), player.getMaxHealth());
        type (
            "\nYou channel magical energy into a healing aura, wrapping yourself in a warm, sooting light."
            "\nYour wounds are mended and you heal ", healing, " points!"
            "\nYour health is now ", player.health, ".\n"
        );
    } else
        return 0;
    return 1;
}

uint16_t PlayerPublic::heal(const uint16_t div) {
    static const float HEALING_MULTIPLIER = randint(10, 15) / 10.f;
    uint16_t healing = (player.getMaxHealth() - player.health) * HEALING_MULTIPLIER / div;
    /*
    function: f(h) = r(m - h) / 2 where
    f(h) = healing
    m = max health
    h = current health
    r = random number between 1.0 and 1.5
    */
    if (player.Race == Player::REVENANT)
        healing /= 2;
    player.health += healing;
    return healing;
}

uint16_t PlayerPublic::healMax() {
    if (player.Race == Player::REVENANT)
        player.health += (player.getMaxHealth() - player.health) / 2;
    else
        player.health = player.getMaxHealth();
    return player.health;
}

void PlayerPublic::addDebuff(Debuff::TYPE debuffType) {
    player.debuffs.push_back(Debuff(debuffType));
    player.health = std::min(player.health, player.getMaxHealth());
    type("You are now ", Debuff::Data[debuffType].name, ".\n");
    type("Your ");
    if (Debuff::Data[debuffType].hMod < 0) {
        type("health is decreased by ", std::abs(Debuff::Data[debuffType].hMod * 100), "%");
    } else if (Debuff::Data[debuffType].hMod > 0) {
        type("health is increased by ", std::abs(Debuff::Data[debuffType].hMod * 100), "%");
    } if (Debuff::Data[debuffType].strMod < 0) {
        type("strength is decreased by ", std::abs(Debuff::Data[debuffType].strMod * 100), "%");
    } else if (Debuff::Data[debuffType].strMod > 0) {
        type("strength is increased by ", std::abs(Debuff::Data[debuffType].strMod * 100), "%");
    } if (Debuff::Data[debuffType].defMod < 0) {
        type("defense is decreased by ", std::abs(Debuff::Data[debuffType].defMod * 100), "%");
    } else if (Debuff::Data[debuffType].defMod > 0) {
        type("defense is increased by ", std::abs(Debuff::Data[debuffType].defMod * 100), "%");
    }
    type(" for ", Debuff::Data[debuffType].duration, " turns.\n");
}

void PlayerPublic::updateDebuffs() {
    for (std::vector<Debuff>::iterator it = player.debuffs.begin(); it != player.debuffs.end(); ) {
        if ((*it).duration == 1) {
            type("\nYou are no longer ", (*it).name, ".\n");
            it = player.debuffs.erase(it);
        } else {
            (*it).duration--;
            it++;
        }
    }
}

void PlayerPublic::displayDebuffs() const {
    type("\nDebuffs:\n");
    if (player.debuffs.empty()) {
        setOutputSettings(true);
        type("\tNone\n");
    } else {
        for (const Debuff& debuff : player.debuffs) {
            type("\t", debuff.name, " (");
            if (debuff.hMod < 0)
                type("Decreased health by ", std::round(player.maxHealth - player.maxHealth * (1 + debuff.hMod)));
            else if (debuff.hMod > 0)
                type("Increased health by ", std::round(player.maxHealth - player.maxHealth * (1 + debuff.hMod)));
            else if (debuff.strMod < 0)
                type("Decreased strength by ", std::round(player.strength - player.strength * (1 + debuff.strMod)));
            else if (debuff.strMod > 0)
                type("Increased strength by ", std::round(player.strength - player.strength * (1 + debuff.strMod)));
            else if (debuff.defMod < 0)
                type("Decreased defense by ", std::round(player.defense - player.defense * (1 + debuff.defMod)));
            else if (debuff.defMod > 0)
                type("Increased defense by ", std::round(player.defense - player.defense * (1 + debuff.defMod)));
            type(" for ", debuff.duration, " turns)\n");
        }
    }
}

void PlayerPublic::initArmor(const Item::Armor& armor, const Item::Source source) {
    armor.displayInfo(source);
    pPrv.equipArmor(armor);
}

void PlayerPublic::initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add) {
    Item::Armor armor(aType, player.level, add);
    initArmor(armor, source);
}

void PlayerPublic::initWeapon(const Item::Weapon& weapon, const Item::Source source) {
    weapon.displayInfo(source);
    pPrv.equipWeapon(weapon);
}

void PlayerPublic::initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add) {
    Item::Weapon weapon(wType, player.level, add);
    initWeapon(weapon, source);
}

void PlayerPublic::initSpecial(const Item::Special& special, const Item::Source source) {
    special.displayInfo(source);
    pPrv.equipSpecial(special);
}

void PlayerPublic::initSpecial(const Item::TYPE sType, const Item::Source source) {
    Item::Special special(sType, player.level);
    initSpecial(special, source);
}

void PlayerPublic::initItem(const Item::Item& item, const Item::Source source) {
    item.displayInfo(source);
    player.resources.addResource(item.name);
}

void PlayerPublic::initItem(const Item::TYPE iType, const Item::Source source) {
    Item::Item item(iType);
    initItem(item, source);
}

void PlayerPublic::unequipArmor(const bool confirmation) {
    if (confirmation) {
        Choice choice;
        do choice = input("Are you sure you want to unequip your ", player.armor.name, " (1. Yes / 2. No)? ");
        while (!choice.isChoice(true, "yes", 1, "no", 2));
        if (choice.isChoice("no", 2))
            return;
        type("You unequip your ", player.armor.name, ".\n");
    }
    player.defense -= player.armor.defenseBonus;
    player.armor = Item::Armor();
}

void PlayerPublic::unequipWeapon(const bool confirmation) {
    if (confirmation) {
        Choice choice;
        do choice = input("Are you sure you want to unequip your ", player.weapon.name, " (1. Yes / 2. No)? ");
        while (!choice.isChoice(true, "yes", 1, "no", 2));
        if (choice.isChoice("no", 2))
            return;
        type("You unequip your ", player.weapon.name, ".\n");
    }
    if (player.weapon == Item::TYPE::WPN_ST_WARBORN) {
        player.maxMana -= 3;
        player.mana = std::max(player.mana - 3, 0);
    } else if (player.weapon == Item::TYPE::WPN_ST_WEEPING) {
        player.maxMana += 3;
        player.mana += 3;
    }
    player.strength -= player.weapon.strengthBonus;
    player.weapon = Item::Weapon();
}

void PlayerPublic::gatherResources() {
    type("\nCollecting resources...\n");
    wheel();

    Resources colResources;
    colResources.addResource("wood", randint(0, player.level))
    ->addResource("stone", randint(0, player.level))
    ->addResource("fiber", randint(0, player.level))
    ->addResource("leather", randint(0, player.level));

    player.resources.addResources(colResources);

    type("Done! Collected resources:\n");
    colResources.displayResources(false);
}

void PlayerPublic::craft() {
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

        if (typeChoice.isChoice("armor", 1)) {
            if (!pPrv.initCraftArmor(items, i))
                continue;
        } else if (typeChoice.isChoice("weapons", 2)) {
            if (!pPrv.initCraftWeapons(items, i)) [[unlikely]]
                continue;
        } else if (typeChoice.isChoice("items", 3)) {
            if (!pPrv.initCraftItems(items, i))
                continue;
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

            switch ((*items[choiceNum]).itemType) {
            case Item::TYPE::ARM_DRAKONIAN: // Drakonian Armor
                pPrv.craftArmor(static_cast<Item::Armor&>(*items[choiceNum]), "fiber", 2, "leather", 6);
                break;
            case Item::TYPE::ARM_LEATHER: // Leather Armor
                pPrv.craftArmor(static_cast<Item::Armor&>(*items[choiceNum]), "fiber", 2, "leather", 6);
                break;
            case Item::TYPE::WPN_LONG: // Longsword
                pPrv.craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "fiber", 2, "iron", 3, "wood", 2);
                break;
            case Item::TYPE::WPN_ST_WARBORN: // Staff of the Warborn
                pPrv.craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Warborn", 1, "fiber", 2, "wood", 4);
                break;
            case Item::TYPE::WPN_ST_GUARDIAN: // Staff of the Guardian
                pPrv.craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Guardian", 1, "fiber", 2, "wood", 4);
                break;
            case Item::TYPE::WPN_ST_SHADOW: // Staff of the Shadow
                pPrv.craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Shadow", 1, "fiber", 2, "wood", 4);
                break;
            case Item::TYPE::WPN_ST_FURY: // Staff of Fury
                pPrv.craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of Fury", 1, "fiber", 2, "wood", 4);
                break;
            case Item::TYPE::WPN_ST_WEEPING: // Staff of the Weeping Spirit
                pPrv.craftWeapon(static_cast<Item::Weapon&>(*items[choiceNum]), "Amulet of the Weeping Spirit", 1, "fiber", 2, "wood", 4);
                break;
            case Item::TYPE::FOCUS: // Arcane Focus
                pPrv.craftItem(*items[choiceNum], "crystals", 4);
                break;
            }
            return;
        }
    } while (true);
}