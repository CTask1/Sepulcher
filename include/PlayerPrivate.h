//CTask1
#pragma once
#include<initializer_list>
#include<unordered_map>
#include<string>
#include<memory>
#include<cmath>

#include"Item.h"

class Player;

class PlayerPrivate {
public:

    PlayerPrivate(Player& p) : player(p) {}
    ~PlayerPrivate() {}

    bool initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const;
    bool initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const;
    bool initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const;
    bool useComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components);
    bool initCraft(const Item::Item* const& item, const std::initializer_list<std::pair<std::string, uint16_t>> components);
    bool craftArmor(Item::Armor& armor, const std::initializer_list<std::pair<std::string, uint16_t>> components);
    bool craftWeapon(const Item::Weapon& weapon, const std::initializer_list<std::pair<std::string, uint16_t>> components);
    bool craftItem(const Item::Item& item, const std::initializer_list<std::pair<std::string, uint16_t>> components);
    void equipArmor(const Item::Armor& armorItem);
    void equipWeapon(const Item::Weapon& weaponItem);
    void equipSpecial(const Item::Special& specialItem);
    const std::string displayComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components) const;

private:
    Player& player;
};