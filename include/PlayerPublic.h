#pragma once
#include"PlayerPrivate.h"
#include"Enemy.h"
#include"Item.h"

class Player;

class PlayerPublic {
public:

    PlayerPublic(Player& p) : player(p), pPrv(p) {}
    ~PlayerPublic() {}

    void receiveGift();
    const bool rituals();
    const bool abilities(Enemy::Enemy*, short*);
    const uint16_t heal(const uint16_t);
    void initArmor(const Item::Armor&, const Item::Source);
    void initArmor(const Item::TYPE, const Item::Source, const uint16_t);
    void initWeapon(const Item::Weapon&, const Item::Source);
    void initWeapon(const Item::TYPE, const Item::Source, const uint16_t);
    void initSpecial(const Item::Special&, const Item::Source);
    void initSpecial(const Item::TYPE, const Item::Source);
    void initItem(const Item::Item&, const Item::Source);
    void initItem(const Item::TYPE, const Item::Source);
    void unequipArmor(const bool);
    void unequipWeapon(const bool);
    void gatherResources();
    void craft();

private:
    Player& player;
    PlayerPrivate pPrv;
};