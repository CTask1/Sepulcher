#include<initializer_list>
#include<unordered_map>
#include<iostream>
#include<memory>
#include<string>
#include<cmath>

#include"..\include\PlayerPrivate.h"
#include"..\include\PlayerPublic.h"
#include"..\include\Player.h"
#include"..\include\Util.h"

Player::Player(std::string_view n, RACE ra, CLASS cl, uint16_t h, uint16_t s, uint16_t co) :
  pPub(std::make_unique<PlayerPublic>(*this)),
  pPrv(std::make_unique<PlayerPrivate>(*this)),
  name(n),
  Race ( {
    R[ra].Race,
    R[ra].name
  } ),
  Class ( {
    C[cl].Class,
    C[cl].name
  } ),
  maxHealth(h),
  health(h),
  maxMana(0),
  mana(0),
  baseStrength(s),
  strength(s),
  CON(co),
  defense(0),
  mageArmorDefense(0),
  level(1),
  nextLevel(10 + static_cast<uint32_t>(pow(level, 2))),
  exp(0),
  bloodMeter(0),
  hasAbility(false),
  raceAbilityReady(false),
  classAbilityReady(false),
  arcaneEye(false) {
    if (Race == DRAKONIAN) {
        hasAbility = true;
        raceAbilityReady = true;
    } else if (Race == REVENANT) {
        hasAbility = true;
        raceAbilityReady = true;
        resurgence = true;
    }
    if (Class == FIGHTER) {
        hasAbility = true;
        classAbilityReady = true;
    } else if (Class == WIZARD) {
        hasAbility = true;
        classAbilityReady = true;
        maxMana = 15;
        mana = 15;
    }
    displayStats();
    resources.displayResources();
}

Player::~Player() = default;

void Player::displayStats() const {
    setOutputSettings(true);
    std::cout << "\n-------------------------";
    type (
        "\nName: ", name,
        "\nRace: ", Race.name,
        (Race == REVENANT ? "\nBlood Meter: " + std::to_string(bloodMeter) + "/3" : ""),
        "\nClass: ", Class.name,
        (Class == WIZARD ? "\nMana Points: " + std::to_string(mana) + '/' + std::to_string(maxMana) : ""),
        "\nHealth: ", health, "/", getMaxHealth(),
        "\nStrength: ", strength,
        "\nConstitution: ", CON,
        "\nDefense: ", defense,
        "\nWeapon: ", weapon.name,
        "\nArmor: ", armor.name,
        "\nSpecial: ", special.name,
        "\nLevel: ", level,
        "\nExperience: ", exp, "/", nextLevel
    );
    std::cout << "\n-------------------------\n";
    displayDebuffs();
}

uint16_t Player::getMaxHealth() const                                                                                              { return (*pPub).getMaxHealth      (                                      ); }
short Player::getStrength() const                                                                                                  { return (*pPub).getStrength       (                                      ); }
short Player::getDefense() const                                                                                                   { return (*pPub).getDefense        (                                      ); }
void Player::receiveGift()                                                                                                         {        (*pPub).receiveGift       (                                      ); }
bool Player::rituals()                                                                                                             { return (*pPub).rituals           (                                      ); }
bool Player::abilities(Enemy::Enemy* enemy, short* mirrorImage, bool* shadowmeld)                                                  { return (*pPub).abilities         ( enemy      , mirrorImage, shadowmeld ); }
uint16_t Player::heal(const uint16_t div)                                                                                          { return (*pPub).heal              ( div                                  ); }
uint16_t Player::healMax()                                                                                                         { return (*pPub).healMax           (                                      ); }
void Player::addDebuff(Debuff::TYPE debuffType)                                                                                    {        (*pPub).addDebuff         ( debuffType                           ); }
void Player::updateDebuffs()                                                                                                       {        (*pPub).updateDebuffs     (                                      ); }
void Player::displayDebuffs() const                                                                                                {        (*pPub).displayDebuffs    (                                      ); }
void Player::initItem(const Item::Item& item, const Item::Source source)                                                           {        (*pPub).initItem          ( item       , source                  ); }
void Player::initItem(const Item::TYPE iType, const Item::Source source)                                                           {        (*pPub).initItem          ( iType      , source                  ); }
void Player::initArmor(const Item::Armor& armorItem, const Item::Source source)                                                    {        (*pPub).initArmor         ( armorItem  , source                  ); }
void Player::initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add)                                      {        (*pPub).initArmor         ( aType      , source     , add        ); }
void Player::initWeapon(const Item::Weapon& weaponItem, const Item::Source source)                                                 {        (*pPub).initWeapon        ( weaponItem , source                  ); }
void Player::initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add)                                     {        (*pPub).initWeapon        ( wType      , source     , add        ); }
void Player::initSpecial(const Item::Special& specialItem, const Item::Source source)                                              {        (*pPub).initSpecial       ( specialItem, source                  ); }
void Player::initSpecial(const Item::TYPE sType, const Item::Source source)                                                        {        (*pPub).initSpecial       ( sType      , source                  ); }
void Player::unequipArmor(const bool confirmation)                                                                                 {        (*pPub).unequipArmor      ( confirmation                         ); }
void Player::unequipWeapon(const bool confirmation)                                                                                {        (*pPub).unequipWeapon     ( confirmation                         ); }
void Player::gatherResources()                                                                                                     {        (*pPub).gatherResources   (                                      ); }
void Player::craft()                                                                                                               {        (*pPub).craft             (                                      ); }
bool Player::initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const                   { return (*pPrv).initCraftArmor    ( items      , i                       ); }
bool Player::initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const                 { return (*pPrv).initCraftWeapons  ( items      , i                       ); }
bool Player::initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const                   { return (*pPrv).initCraftItems    ( items      , i                       ); }
bool Player::useComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components)                               { return (*pPrv).useComponents     ( components                           ); }
bool Player::initCraft(const Item::Item* const& item, const std::initializer_list<std::pair<std::string, uint16_t>> components)    { return (*pPrv).initCraft         ( item       , components              ); }
bool Player::craftArmor(Item::Armor& armorItem, const std::initializer_list<std::pair<std::string, uint16_t>> components)          { return (*pPrv).craftArmor        ( armorItem  , components              ); }
bool Player::craftWeapon(const Item::Weapon& weaponItem, const std::initializer_list<std::pair<std::string, uint16_t>> components) { return (*pPrv).craftWeapon       ( weaponItem , components              ); }
bool Player::craftItem(const Item::Item& item, const std::initializer_list<std::pair<std::string, uint16_t>> components)           { return (*pPrv).craftItem         ( item       , components              ); }
void Player::equipArmor(const Item::Armor& armorItem)                                                                              {        (*pPrv).equipArmor        ( armorItem                            ); }
void Player::equipWeapon(const Item::Weapon& weaponItem)                                                                           {        (*pPrv).equipWeapon       ( weaponItem                           ); }
void Player::equipSpecial(const Item::Special& specialItem)                                                                        {        (*pPrv).equipSpecial      ( specialItem                          ); }
const std::string Player::displayComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components) const        { return (*pPrv).displayComponents ( components                           ); } 