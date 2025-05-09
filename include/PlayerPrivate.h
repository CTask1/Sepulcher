//CTask
#pragma once
#include<initializer_list>
#include<memory>
#include<string>
#include<vector>

#include"Item.h"
#include"util.h"

typedef std::initializer_list<std::pair<std::string, uint16_t>> PairList_t;
typedef std::unique_ptr<Item::Item> ItemPtr_t;
typedef std::vector<ItemPtr_t> ItemPtrVec_t;

class Player;

class PlayerPrivate {
public:

    PlayerPrivate(Player& p) : player(p) {}
    ~PlayerPrivate() = default;

    bool initCraftArmor(ItemPtrVec_t& items, uint16_t& i) const;
    bool initCraftWeapons(ItemPtrVec_t& items, uint16_t& i) const;
    bool initCraftGeneral(ItemPtrVec_t& items, uint16_t& i) const;
    bool useComponents(const PairList_t& components);
    bool craftItem(const ItemPtr_t& item, const PairList_t& components);
    void equipArmor(const Item::Armor& armorItem);
    void equipWeapon(const Item::Weapon& weaponItem);
    void equipSpecial(const Item::Special& specialItem);
    const std::string displayComponents(const PairList_t& components) const;

private:
    Player& player;
};