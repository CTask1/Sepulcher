#pragma once
#include"PlayerPrivate.h"
#include"Player.h"

template<typename... Args>
bool PlayerPrivate::checkComponents(const std::string& resource, const uint16_t amount, const Args&... args) {
    return player.resources[resource] >= amount && checkComponents(args...);
}

template<typename... Args>
void PlayerPrivate::useComponents(const std::string& resource, const uint16_t amount, const Args&... args) {
    player.resources[resource] -= amount;
    useComponents(args...);
}

template<typename... Args>
bool PlayerPrivate::initCraft(const Item::Item* const& item, const Args&... args) {
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
bool PlayerPrivate::craftArmor(Item::Armor& armor, const Args&... args) {
    if (!initCraft(&armor, args...))
        return 0;
    player.initArmor(armor, Item::Source::CRAFT);
    return 1;
}

template<typename... Args>
bool PlayerPrivate::craftWeapon(const Item::Weapon& weapon, const Args&... args) {
    if (!initCraft(&weapon, args...))
        return 0;
    player.initWeapon(weapon, Item::Source::CRAFT);
    return 1;
}

template<typename... Args>
bool PlayerPrivate::craftItem(const Item::Item& item, const Args&... args) {
    if (!initCraft(&item, args...))
        return 0;
    player.initItem(item, Item::Source::CRAFT);
    return 1;
}

template<typename... Args>
const std::string PlayerPrivate::getComponents(const std::string& name, const uint16_t amt, const Args&... args) {
    return "\n\t" + ((player.resources.resources.count(name) != 0) ? (name + ": " + std::to_string(player.resources[name]) + '/' + std::to_string(amt)) : "??") + getComponents(args...);
}

template<typename... Args>
const std::string PlayerPrivate::displayComponents(const Args&... args) {
    return "\nComponents:" + getComponents(args...);
}