#pragma once
#include"Item.h"

class Player;

class PlayerPrivate {
public:

    PlayerPrivate(Player& p) : player(p) {}
    ~PlayerPrivate() {}

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

private:
    Player& player;
};

#include"PlayerPrivate.tpp"