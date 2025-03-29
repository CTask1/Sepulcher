#include<initializer_list>

#include"..\include\PlayerPrivate.h"
#include"..\include\Player.h"
#include"..\include\Item.h"
#include"..\include\Util.h"

bool PlayerPrivate::initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const {
    if (player.Class == Player::WIZARD) {
        type("There is no armor available to craft.\n");
        return false;
    }
    if (player.Race == Player::DRAKONIAN) {
        Item::Armor drakArmor(Item::TYPE::ARM_DRAKONIAN, player.level, 1, true);
        items[i] = std::make_unique<Item::Armor>(std::move(drakArmor));
        type (
            ++i, ". Drakonian Armor (Defense Bonus: ", drakArmor.defenseBonus, ")",
            displayComponents({ { "fiber", (uint16_t)2 }, { "leather", (uint16_t)6 } }), "\n"
        );
    } else {
        Item::Armor leatherArmor(Item::TYPE::ARM_LEATHER, player.level, 1, true);
        items[i] = std::make_unique<Item::Armor>(std::move(leatherArmor));
        type (
            ++i, ". Leather Armor (Defense Bonus: ", leatherArmor.defenseBonus, ")",
            displayComponents({ { "fiber", (uint16_t)2 }, { "leather", (uint16_t)6 } }), "\n"
        );
    }
    return true;
}

bool PlayerPrivate::initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const {
    Item::Weapon longsword(Item::TYPE::WPN_LONG, player.level, (player.Class == Player::WIZARD ? -1 : 1), true);
    items[i] = std::make_unique<Item::Weapon>(std::move(longsword));
    type (
        ++i, ". Longsword (Strength Bonus: ", longsword.strengthBonus, ")",
        displayComponents({ { "fiber", (uint16_t)2 }, { "iron", (uint16_t)3 }, { "wood", (uint16_t)2 } }), "\n"
    );
    if (player.Class == Player::WIZARD) {
        Item::Weapon stWarborn(Item::TYPE::WPN_ST_WARBORN, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::make_unique<Item::Weapon>(std::move(stWarborn));
        type (
            ++i, ". Staff of the Warborn (Strength Bonus: ", stWarborn.strengthBonus, ")",
            displayComponents({ { "Amulet of the Warborn", (uint16_t)1 }, { "fiber", (uint16_t)2 }, { "wood", (uint16_t)4 } }), "\n"
        );
        Item::Weapon stGuardian(Item::TYPE::WPN_ST_GUARDIAN, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::make_unique<Item::Weapon>(std::move(stGuardian));
        type (
            ++i, ". Staff of the Guardian (Strength Bonus: ", stGuardian.strengthBonus, ")",
            displayComponents({ { "Amulet of the Guardian", (uint16_t)1 }, { "fiber", (uint16_t)2 }, { "wood", (uint16_t)4 } }), "\n"
        );
        Item::Weapon stShadow(Item::TYPE::WPN_ST_SHADOW, player.level, 1, true);
        items[i] = std::make_unique<Item::Weapon>(std::move(stShadow));
        type (
            ++i, ". Staff of the Shadow (Strength Bonus: ", stShadow.strengthBonus, ")",
            displayComponents({ { "Amulet of the Shadow", (uint16_t)1 }, { "fiber", (uint16_t)2 }, { "wood", (uint16_t)4 } }), "\n"
        );
        Item::Weapon stFury(Item::TYPE::WPN_ST_FURY, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::make_unique<Item::Weapon>(std::move(stFury));
        type (
            ++i, ". Staff of Fury (Strength Bonus: ", stFury.strengthBonus, ")",
            displayComponents({ { "Amulet of Fury", (uint16_t)1 }, { "fiber", (uint16_t)2 }, { "wood", (uint16_t)4 } }), "\n"
        );
        Item::Weapon stWeeping(Item::TYPE::WPN_ST_WEEPING, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::make_unique<Item::Weapon>(std::move(stWeeping));
        type (
            ++i, ". Staff of the Weeping Spirit (Strength Bonus: ", stWeeping.strengthBonus, ")",
            displayComponents({ { "Amulet of the Weeping Spirit", (uint16_t)1 }, { "fiber", (uint16_t)2 }, { "wood", (uint16_t)4 } }), "\n"
        );
    }
    return true;
}

bool PlayerPrivate::initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const {
    if (player.Class != Player::WIZARD) {
        type("There are no items available to craft.\n");
        return false;
    }
    Item::Item focus(Item::TYPE::FOCUS);
    items[i] = std::make_unique<Item::Item>(std::move(focus));
    type (
        ++i, ". Arcane Focus",
        displayComponents({ { "crystals", (uint16_t)4 } }), "\n"
    );
    return true;
}

bool PlayerPrivate::useComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components) {
    for (const std::pair<std::string, uint16_t>& component : components) {
        if (player.resources[component.first] < component.second) {
            type("You don't have enough resources!\n");
            return false;
        }
        player.resources[component.first] -= component.second;
    }
    return true;
}

bool PlayerPrivate::initCraft(const Item::Item* const& item, const std::initializer_list<std::pair<std::string, uint16_t>> components) {
    if (!useComponents(components))
        return false;
    type("\nCrafting ", (*item).name, "...\n");
    wheel();
    return true;
}

bool PlayerPrivate::craftArmor(Item::Armor& armor, const std::initializer_list<std::pair<std::string, uint16_t>> components) {
    if (!initCraft(&armor, components))
        return false;
    player.initArmor(armor, Item::Source::CRAFT);
    return true;
}

bool PlayerPrivate::craftWeapon(const Item::Weapon& weapon, const std::initializer_list<std::pair<std::string, uint16_t>> components) {
    if (!initCraft(&weapon, components))
        return false;
    player.initWeapon(weapon, Item::Source::CRAFT);
    return true;
}

bool PlayerPrivate::craftItem(const Item::Item& item, const std::initializer_list<std::pair<std::string, uint16_t>> components) {
    if (!initCraft(&item, components))
        return false;
    player.initItem(item, Item::Source::CRAFT);
    return true;
}

void PlayerPrivate::equipArmor(const Item::Armor& armorItem) {
    if (player.Race == Player::DRAKONIAN && armorItem != Item::TYPE::ARM_DRAKONIAN) {
        type("Your unique body shape renders you unable to wear this armor.\n");
        return;
    }
    if (player.Class == Player::WIZARD) {
        type("The weight and rigidity of this armor would interfere with your connection to the arcane, preventing you from casting spells while wearing it.\n");
        return;
    }
    if (player.armor != Item::TYPE::NONE)
        type("Your current ", player.armor.name, " has (Defense Bonus: ", player.armor.defenseBonus, ")\n");

    type("\nDo you want to equip the ", armorItem.name, "? ");
    Choice choice;
    do choice = input("(1. Yes / 2. No): ");
    while (!choice.isChoice(true, "yes", 1, "no", 2));

    if (choice.isChoice("yes", 1)) {
        type("You equip the ", armorItem.name, ".\n");
        player.unequipArmor(false);
        player.defense += armorItem.defenseBonus;
        player.armor = armorItem;
    } else
        type("You choose to leave the ", armorItem.name, ".\n");
}

void PlayerPrivate::equipWeapon(const Item::Weapon& weaponItem) {
    if (player.weapon != Item::TYPE::NONE)
        type("Your current ", player.weapon.name, " has (Strength Bonus: ", player.weapon.strengthBonus, ")\n");

    type("\nDo you want to equip the ", weaponItem.name, "? ");
    Choice choice;
    do choice = input("(1. Yes / 2. No): ");
    while (!choice.isChoice(true, "yes", 1, "no", 2));

    if (choice.isChoice("yes", 1)) {
        type("You equip the ", weaponItem.name, ".\n");
        player.unequipWeapon(false);
        player.strength += weaponItem.strengthBonus;
        player.weapon = weaponItem;
        if (player.weapon == Item::TYPE::WPN_ST_WARBORN) {
            player.maxMana += 3;
            player.mana += 3;
        } else if (player.weapon == Item::TYPE::WPN_ST_WEEPING) {
            player.maxMana -= 3;
            player.mana = (uint16_t)std::max(player.mana - 3, 0);
        }
    } else
        type("You choose to leave the ", weaponItem.name, ".\n");
}

void PlayerPrivate::equipSpecial(const Item::Special& specialItem) {
    if (player.special != Item::TYPE::NONE) {
        type("A brittle snap splits the air as your old ", player.special.name, " crumbles to dust.\n");
        player.resources[player.special.name] = 0;
    }
    player.defense -= player.special.defenseBonus;
    player.strength -= player.special.strengthBonus;
    player.defense += specialItem.defenseBonus;
    player.strength += player.special.strengthBonus;
    player.special = specialItem;
}

const std::string PlayerPrivate::displayComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components) const {
    std::string result = "\nComponents:";
    for (const std::pair<std::string, uint16_t>& component : components) {
        if (player.resources.resources.count(component.first) != 0)
            result += "\n\t" + component.first + ": " + std::to_string(player.resources[component.first]) + '/' + std::to_string(component.second);
        else
            result += "\n\t??";
    }
    return result;
}