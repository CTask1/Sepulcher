//CTask
#include<initializer_list>
#include<memory>
#include<string>
#include<vector>

#include"Choice.h"
#include"PlayerPrivate.h"
#include"Player.h"
#include"Item.h"
#include"util.h"

bool PlayerPrivate::initCraftArmor(ItemPtrVec_t& items, uint16_t& i) const {
    if (player.Class == Player::CLASS::WIZARD) {
        type("There is no armor available to craft.\n");
        return false;
    }
    if (player.Race == Player::RACE::DRAKONIAN) {
        items.emplace_back(std::make_unique<Item::Armor>(Item::TYPE::ARM_DRAKONIAN, player.level, shrt(1), true));
        type (
            ++i, ". Drakonian Armor (Defense Bonus: ", Item::Armor::getStats(Item::TYPE::ARM_DRAKONIAN, player.level), ")",
            displayComponents({ { "Fiber", ui16(2) }, { "Leather", ui16(6) } }), "\n"
        );
    } else {
        items.emplace_back(std::make_unique<Item::Armor>(Item::TYPE::ARM_LEATHER, player.level, shrt(1), true));
        type (
            ++i, ". Leather Armor (Defense Bonus: ", Item::Armor::getStats(Item::TYPE::ARM_LEATHER, player.level), ")",
            displayComponents({ { "Fiber", ui16(2) }, { "Leather", ui16(6) } }), "\n"
        );
    }
    return true;
}

bool PlayerPrivate::initCraftWeapons(ItemPtrVec_t& items, uint16_t& i) const {
    items.emplace_back(std::make_unique<Item::Weapon>(Item::TYPE::WPN_LONG, player.level, shrt(player.Class == Player::CLASS::WIZARD ? -1 : 1), true));
    type (
        ++i, ". Longsword (Strength Bonus: ", Item::Weapon::getStats(Item::TYPE::WPN_LONG, player.level), ")",
        displayComponents({ { "Fiber", ui16(2) }, { "Iron", ui16(3) }, { "Wood", ui16(2) } }), "\n"
    );
    if (player.Class == Player::CLASS::WIZARD) {
        items.emplace_back(std::make_unique<Item::Weapon>(Item::TYPE::WPN_ST_WARBORN, player.level, shrt(player.Race == Player::RACE::ELF ? 2 : 1), true));
        type (
            ++i, ". Staff of the Warborn (Strength Bonus: ", Item::Weapon::getStats(Item::TYPE::WPN_ST_WARBORN, player.level), ")",
            displayComponents({ { "Amulet of the Warborn", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }), "\n"
        );
        items.emplace_back(std::make_unique<Item::Weapon>(Item::TYPE::WPN_ST_GUARDIAN, player.level, shrt(player.Race == Player::RACE::ELF ? 2 : 1), true));
        type (
            ++i, ". Staff of the Guardian (Strength Bonus: ", Item::Weapon::getStats(Item::TYPE::WPN_ST_GUARDIAN, player.level), ")",
            displayComponents({ { "Amulet of the Guardian", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }), "\n"
        );
        items.emplace_back(std::make_unique<Item::Weapon>(Item::TYPE::WPN_ST_SHADOW, player.level, shrt(1), true));
        type (
            ++i, ". Staff of the Shadow (Strength Bonus: ", Item::Weapon::getStats(Item::TYPE::WPN_ST_SHADOW, player.level), ")",
            displayComponents({ { "Amulet of the Shadow", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }), "\n"
        );
        items.emplace_back(std::make_unique<Item::Weapon>(Item::TYPE::WPN_ST_FURY, player.level, shrt(player.Race == Player::RACE::ELF ? 2 : 1), true));
        type (
            ++i, ". Staff of Fury (Strength Bonus: ", Item::Weapon::getStats(Item::TYPE::WPN_ST_FURY, player.level), ")",
            displayComponents({ { "Amulet of Fury", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }), "\n"
        );
        items.emplace_back(std::make_unique<Item::Weapon>(Item::TYPE::WPN_ST_WEEPING, player.level, shrt(player.Race == Player::RACE::ELF ? 2 : 1), true));
        type (
            ++i, ". Staff of the Weeping Spirit (Strength Bonus: ", Item::Weapon::getStats(Item::TYPE::WPN_ST_WEEPING, player.level), ")",
            displayComponents({ { "Amulet of the Weeping Spirit", ui16(1) }, { "Fiber", ui16(2) }, { "Wood", ui16(4) } }), "\n"
        );
    }
    return true;
}

bool PlayerPrivate::initCraftGeneral(ItemPtrVec_t& items, uint16_t& i) const {
    items.emplace_back(std::make_unique<Item::Item>(Item::TYPE::POTION));
    type (
        ++i, ". Health Potion",
        displayComponents({ { "Medicinal Herbs", ui16(2) } }), "\n"
    );
    
    if (player.Class == Player::CLASS::WIZARD) {
        items.emplace_back(std::make_unique<Item::Item>(Item::TYPE::FOCUS));
        type (
            ++i, ". Arcane Focus",
            displayComponents({ { "Crystals", ui16(4) } }), "\n"
        );
    }
    return true;
}

bool PlayerPrivate::useComponents(const PairList_t& components) {
    for (const std::pair<std::string, uint16_t>& component : components) {
        if (player.resources[component.first] < component.second) {
            type("You don't have enough resources!\n");
            return false;
        }
        player.resources[component.first] -= component.second;
    }
    return true;
}

bool PlayerPrivate::craftItem(const ItemPtr_t& item, const PairList_t& components) {
    if (!useComponents(components))
        return false;
    type("\nCrafting ", item->name, "...\n");
    wheel();
    switch (Item::Data[ui16(item->itemType)].type) {
    case Item::ItemClass::ARM:
        player.initArmor(static_cast<const Item::Armor&>(*item), Item::Source::CRAFT);
        break;
    case Item::ItemClass::WPN:
        player.initWeapon(static_cast<const Item::Weapon&>(*item), Item::Source::CRAFT);
        if (!Item::Data[ui16(item->itemType)].canHavePrefix) // If the item is a staff
            equipSpecial(Item::Special()); // Remove the player's amulet
        break;
    default:
        player.initGeneral(static_cast<const Item::General&>(*item), Item::Source::CRAFT);
        break;
    }
    return true;
}

void PlayerPrivate::equipArmor(const Item::Armor& armorItem) {
    if (player.Race == Player::RACE::DRAKONIAN && armorItem != Item::TYPE::ARM_DRAKONIAN) {
        type("Your unique body shape renders you unable to wear this armor.\n");
        return;
    }
    if (player.Class == Player::CLASS::WIZARD) {
        type("The weight and rigidity of this armor would interfere with your connection to the arcane, preventing you from casting spells while wearing it.\n");
        return;
    }
    if (player.armor != Item::TYPE::NONE)
        type("Your current ", player.armor.name, " has (Defense Bonus: ", player.armor.defenseBonus, ")\n");

    type("\nDo you want to equip the ", armorItem.name, " (1. Yes / 2. No)?\n");
    int choice;
    do choice = Choice(input(prompt.data())).isChoice({"yes", "no"});
    while (choice == 0);

    if (choice == 2) {
        type("You choose to leave the ", armorItem.name, ".\n");
        return;
    }
    type("You equip the ", armorItem.name, ".\n");
    player.unequipArmor(false);
    player.defense += armorItem.defenseBonus;
    player.armor = armorItem;
}

void PlayerPrivate::equipWeapon(const Item::Weapon& weaponItem) {
    if (player.weapon != Item::TYPE::NONE)
        type("Your current ", player.weapon.name, " has (Strength Bonus: ", player.weapon.strengthBonus, ")\n");

    type("\nDo you want to equip the ", weaponItem.name, " (1. Yes / 2. No)?\n");
    int choice;
    do choice = Choice(input(prompt.data())).isChoice({"yes", "no"});
    while (choice == 0);

    if (choice == 2) {
        type("You choose to leave the ", weaponItem.name, ".\n");
        return;
    }
    type("You equip the ", weaponItem.name, ".\n");
    player.unequipWeapon(false);
    player.strength += weaponItem.strengthBonus;
    player.weapon = weaponItem;
    if (player.weapon == Item::TYPE::WPN_ST_WARBORN) {
        player.maxMana += 3;
        player.mana += 3;
    } else if (player.weapon == Item::TYPE::WPN_ST_WEEPING) {
        player.maxMana -= 3;
        player.mana = ui16(std::max(player.mana - 3, 0));
    }
}

void PlayerPrivate::equipSpecial(const Item::Special& specialItem) {
    if (player.special != Item::TYPE::NONE && specialItem != Item::TYPE::NONE) {
        type("A brittle snap splits the air as your old ", player.special.name, " crumbles to dust.\n");
        player.resources[player.special.name] = 0;
    }
    player.defense -= player.special.defenseBonus;
    player.strength -= player.special.strengthBonus;
    player.defense += specialItem.defenseBonus;
    player.strength += specialItem.strengthBonus;
    player.special = specialItem;
}

const std::string PlayerPrivate::displayComponents(const PairList_t& components) const {
    std::string result = "\nComponents:";
    for (const std::pair<std::string, uint16_t>& component : components) {
        if (player.resources.resources.count(component.first) != 0)
            result += "\n\t" + component.first + ": " + std::to_string(player.resources[component.first]) + '/' + std::to_string(component.second);
        else
            result += "\n\t??";
    }
    return result;
}