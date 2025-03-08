#include"PlayerPrivate.h"
#include"Player.h"

const bool PlayerPrivate::initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) {
    if (player.Class == Player::WIZARD) {
        type("There is no armor available to craft.\n");
        return 0;
    }
    if (player.Race == Player::DRAKONIAN) {
        Item::Armor drakArmor(Item::TYPE::ARM_DRAKONIAN, player.level, 1, true);
        items[i] = std::move(std::make_unique<Item::Armor>(drakArmor));
        type (
            ++i, ". Drakonian Armor (Defense Bonus: ", drakArmor.defenseBonus, ")",
            displayComponents("fiber", 2, "leather", 6), "\n"
        );
    } else {
        Item::Armor leatherArmor(Item::TYPE::ARM_LEATHER, player.level, 1, true);
        items[i] = std::move(std::make_unique<Item::Armor>(leatherArmor));
        type (
            ++i, ". Leather Armor (Defense Bonus: ", leatherArmor.defenseBonus, ")",
            displayComponents("fiber", 2, "leather", 6), "\n"
        );
    }
    return 1;
}

const bool PlayerPrivate::initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) {
    Item::Weapon longsword(Item::TYPE::WPN_LONG, player.level, (player.Class == Player::WIZARD ? -1 : 1), true);
    items[i] = std::move(std::make_unique<Item::Weapon>(longsword));
    type (
        ++i, ". Longsword (Strength Bonus: ", longsword.strengthBonus, ")",
        displayComponents("fiber", 2, "iron", 3, "wood", 2), "\n"
    );
    if (player.Class == Player::WIZARD) {
        Item::Weapon stWarborn(Item::TYPE::WPN_ST_WARBORN, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::move(std::make_unique<Item::Weapon>(stWarborn));
        type (
            ++i, ". Staff of the Warborn (Strength Bonus: ", stWarborn.strengthBonus, ")",
            displayComponents("Amulet of the Warborn", 1, "fiber", 2, "wood", 4), "\n"
        );
        Item::Weapon stGuardian(Item::TYPE::WPN_ST_GUARDIAN, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::move(std::make_unique<Item::Weapon>(stGuardian));
        type (
            ++i, ". Staff of the Guardian (Strength Bonus: ", stGuardian.strengthBonus, ")",
            displayComponents("Amulet of the Guardian", 1, "fiber", 2, "wood", 4), "\n"
        );
        Item::Weapon stShadow(Item::TYPE::WPN_ST_SHADOW, player.level, 1, true);
        items[i] = std::move(std::make_unique<Item::Weapon>(stShadow));
        type (
            ++i, ". Staff of the Shadow (Strength Bonus: ", stShadow.strengthBonus, ")",
            displayComponents("Amulet of the Shadow", 1, "fiber", 2, "wood", 4), "\n"
        );
        Item::Weapon stFury(Item::TYPE::WPN_ST_FURY, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::move(std::make_unique<Item::Weapon>(stFury));
        type (
            ++i, ". Staff of Fury (Strength Bonus: ", stFury.strengthBonus, ")",
            displayComponents("Amulet of Fury", 1, "fiber", 2, "wood", 4), "\n"
        );
        Item::Weapon stWeeping(Item::TYPE::WPN_ST_WEEPING, player.level, (player.Race == Player::ELF ? 2 : 1), true);
        items[i] = std::move(std::make_unique<Item::Weapon>(stWeeping));
        type (
            ++i, ". Staff of the Weeping Spirit (Strength Bonus: ", stWeeping.strengthBonus, ")",
            displayComponents("Amulet of the Weeping Spirit", 1, "fiber", 2, "wood", 4), "\n"
        );
    }
    return 1;
}

const bool PlayerPrivate::initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) {
    if (player.Class != Player::WIZARD) {
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

const bool PlayerPrivate::checkComponents() const {
    return true;
}

/*template<typename... Args>
const bool PlayerPrivate::checkComponents(const std::string& resource, const uint16_t amount, const Args&... args) {
    return player.resources[resource] >= amount && checkComponents(args...);
}*/

void PlayerPrivate::useComponents() {}

/*template<typename... Args>
void PlayerPrivate::useComponents(const std::string& resource, const uint16_t amount, const Args&... args) {
    player.resources[resource] -= amount;
    useComponents(args...);
}

template<typename... Args>
const bool PlayerPrivate::initCraft(const Item::Item* const& item, const Args&... args) {
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
const bool PlayerPrivate::craftArmor(Item::Armor& armor, const Args&... args) {
    if (!initCraft(&armor, args...))
        return 0;
    player.initArmor(armor, Item::Source::CRAFT);
    return 1;
}

template<typename... Args>
const bool PlayerPrivate::craftWeapon(const Item::Weapon& weapon, const Args&... args) {
    if (!initCraft(&weapon, args...))
        return 0;
    player.initWeapon(weapon, Item::Source::CRAFT);
    return 1;
}

template<typename... Args>
const bool PlayerPrivate::craftItem(const Item::Item& item, const Args&... args) {
    if (!initCraft(&item, args...))
        return 0;
    player.initItem(item, Item::Source::CRAFT);
    return 1;
}*/

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
            player.mana = std::max(player.mana - 3, 0);
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

const std::string PlayerPrivate::getComponents() const { return ""; }

/*template<typename... Args>
const std::string PlayerPrivate::getComponents(const std::string& name, const uint16_t& amt, const Args&... args) {
    return "\n\t" + ((player.resources.resources.count(name) != 0) ? (name + ": " + std::to_string(player.resources[name]) + '/' + std::to_string(amt)) : "??") + getComponents(args...);
}

template<typename... Args>
const std::string PlayerPrivate::displayComponents(const Args&... args) {
    return "\nComponents:" + getComponents(args...);
}*/