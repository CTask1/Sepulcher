//CTask
#pragma once
#include"Player.h"
#include"Enemy.h"
#include"Item.h"

class Player;
class PlayerPrivate;

class PlayerPublic {
public:

    PlayerPublic(Player& p) : player(p), pPrv(p) {}
    ~PlayerPublic() {}

    void addExp(uint16_t);
    void levelUp(uint16_t);
    uint16_t getMaxHealth() const;
    short getStrength() const;
    short getDefense() const;
    void receiveGift();
    bool rituals();
    bool abilities(Enemy::Enemy*, short*, bool*);
    uint16_t heal(float);
    uint16_t healMax();
    void addDebuff(Debuff::TYPE);
    void updateDebuffs();
    void displayDebuffs() const;
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