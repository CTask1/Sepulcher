#pragma once
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

    bool initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i);
    bool initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i);
    bool initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i);
    bool checkComponents() const;
    template<typename... Args> bool checkComponents(const std::string& resource, const uint16_t amount, const Args&... args);
    void useComponents();
    template<typename... Args> void useComponents(const std::string& resource, const uint16_t amount, const Args&... args);
    template<typename... Args> bool initCraft(const Item::Item* const& item, const Args&... args);
    template<typename... Args> bool craftArmor(Item::Armor& armor, const Args&... args);
    template<typename... Args> bool craftWeapon(const Item::Weapon& weapon, const Args&... args);
    template<typename... Args> bool craftItem(const Item::Item& item, const Args&... args);
    void equipArmor(const Item::Armor& armorItem);
    void equipWeapon(const Item::Weapon& weaponItem);
    void equipSpecial(const Item::Special& specialItem);
    const std::string getComponents() const;
    template<typename... Args> const std::string getComponents(const std::string& name, const uint16_t amt, const Args&... args);
    template<typename... Args> const std::string displayComponents(const Args&... args);

private:
    Player& player;
};

#include"PlayerPrivate.tpp"