//CTask
#include<initializer_list>
#include<iostream>
#include<memory>
#include<string>
#include<vector>

#include"PlayerPrivate.h"
#include"PlayerPublic.h"
#include"Player.h"
#include"util.h"

Player::Player(std::string_view n, RACE ra, CLASS cl, uint16_t h, uint16_t s, uint16_t co) :
  pPub(std::make_unique<PlayerPublic>(*this)),
  pPrv(std::make_unique<PlayerPrivate>(*this)),
  name(n),
  Race ( {
    RaceInfo[ui16(ra)].Race,
    RaceInfo[ui16(ra)].name
  } ),
  Class ( {
    ClassInfo[ui16(cl)].Class,
    ClassInfo[ui16(cl)].name
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
  nextLevel(11),
  exp(0),
  bloodMeter(0),
  hasAbility(false),
  raceAbilityReady(false),
  classAbilityReady(false),
  resurgence(false),
  arcaneEye(false) {
    if (Race == RACE::DRAKONIAN) {
        hasAbility = true;
        raceAbilityReady = true;
    } else if (Race == RACE::REVENANT) {
        hasAbility = true;
        raceAbilityReady = true;
        resurgence = true;
    }
    if (Class == CLASS::FIGHTER) {
        hasAbility = true;
        classAbilityReady = true;
    } else if (Class == CLASS::WIZARD) {
        hasAbility = true;
        classAbilityReady = true;
        maxMana = 15;
        mana = 15;
    }
    displayStats();
    resources.displayResources();
}

Player::~Player() = default;

void Player::displayStats(const bool showResources) const {
    setList(true);
    std::cout << "\n-------------------------";
    type (
        "\nName: ", name,
        "\nRace: ", Race.name,
        (Race == RACE::REVENANT ? "\nBlood Meter: " + std::to_string(bloodMeter) + "/3" : ""),
        "\nClass: ", Class.name,
        (Class == CLASS::WIZARD ? "\nMana Points: " + std::to_string(mana) + '/' + std::to_string(maxMana) : ""),
        "\nHealth: ", health, "/", getMaxHealth(),
        "\nStrength: ", strength,
        "\nConstitution: ", CON,
        "\nDefense: ", defense,
        "\nWeapon: ", weapon.name, " (Level: ", weapon.level, ", Strength Bonus: ", weapon.strengthBonus, ")",
        "\nArmor: ", armor.name, " (Level: ", armor.level, ", Defense Bonus: ", armor.defenseBonus, ")",
        "\nSpecial: ", special.name, " (Defense Bonus: ", special.defenseBonus, ", Strength Bonus: ", special.strengthBonus, ")",
        "\nLevel: ", level,
        "\nExperience: ", exp, "/", nextLevel
    );
    std::cout << "\n-------------------------\n";
    if (!debuffs.empty())
        displayDebuffs();
    if (showResources)
        resources.displayResources();
}

// ---------- Public Methods ----------
void Player::addExp(uint16_t xp)                                                               {        pPub->addExp            ( xp                                   ); }
void Player::levelUp(uint16_t hitdie)                                                          {        pPub->levelUp           ( hitdie                               ); }
uint16_t Player::getMaxHealth() const                                                          { return pPub->getMaxHealth      (                                      ); }
short Player::getStrength() const                                                              { return pPub->getStrength       (                                      ); }
short Player::getDefense() const                                                               { return pPub->getDefense        (                                      ); }
void Player::receiveGift()                                                                     {        pPub->receiveGift       (                                      ); }
bool Player::rituals()                                                                         { return pPub->rituals           (                                      ); }
bool Player::abilities(Enemy::Enemy* enemy, short* mirrorImage, bool* shadowmeld)              { return pPub->abilities         ( enemy      , mirrorImage, shadowmeld ); }
uint16_t Player::heal(float div)                                                               { return pPub->heal              ( div                                  ); }
uint16_t Player::healMax()                                                                     { return pPub->healMax           (                                      ); }
void Player::addDebuff(Debuff::TYPE debuffType)                                                {        pPub->addDebuff         ( debuffType                           ); }
void Player::removeDebuff(Debuff::TYPE debuffType)                                             {        pPub->removeDebuff      ( debuffType                           ); }
void Player::updateDebuffs()                                                                   {        pPub->updateDebuffs     (                                      ); }
void Player::displayDebuffs() const                                                            {        pPub->displayDebuffs    (                                      ); }
void Player::initGeneral(const Item::General& generalItem, const Item::Source source)          {        pPub->initGeneral       ( generalItem, source                  ); }
void Player::initGeneral(const Item::TYPE iType, const Item::Source source)                    {        pPub->initGeneral       ( iType      , source                  ); }
void Player::initArmor(const Item::Armor& armorItem, const Item::Source source)                {        pPub->initArmor         ( armorItem  , source                  ); }
void Player::initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add)  {        pPub->initArmor         ( aType      , source     , add        ); }
void Player::initWeapon(const Item::Weapon& weaponItem, const Item::Source source)             {        pPub->initWeapon        ( weaponItem , source                  ); }
void Player::initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add) {        pPub->initWeapon        ( wType      , source     , add        ); }
void Player::initSpecial(const Item::Special& specialItem, const Item::Source source)          {        pPub->initSpecial       ( specialItem, source                  ); }
void Player::initSpecial(const Item::TYPE sType, const Item::Source source)                    {        pPub->initSpecial       ( sType      , source                  ); }
void Player::unequipArmor(const bool confirmation)                                             {        pPub->unequipArmor      ( confirmation                         ); }
void Player::unequipWeapon(const bool confirmation)                                            {        pPub->unequipWeapon     ( confirmation                         ); }
void Player::gatherResources()                                                                 {        pPub->gatherResources   (                                      ); }
void Player::craft()                                                                           {        pPub->craft             (                                      ); }
// ---------- Private Methods ----------
bool Player::initCraftArmor(ItemPtrVec_t& items, uint16_t& i) const                            { return pPrv->initCraftArmor    ( items      , i                       ); }
bool Player::initCraftWeapons(ItemPtrVec_t& items, uint16_t& i) const                          { return pPrv->initCraftWeapons  ( items      , i                       ); }
bool Player::initCraftGeneral(ItemPtrVec_t& items, uint16_t& i) const                          { return pPrv->initCraftGeneral  ( items      , i                       ); }
bool Player::useComponents(const PairList_t& components)                                       { return pPrv->useComponents     ( components                           ); }
bool Player::craftItem(const ItemPtr_t& item, const PairList_t& components)                    { return pPrv->craftItem         ( item       , components              ); }
void Player::equipArmor(const Item::Armor& armorItem)                                          {        pPrv->equipArmor        ( armorItem                            ); }
void Player::equipWeapon(const Item::Weapon& weaponItem)                                       {        pPrv->equipWeapon       ( weaponItem                           ); }
void Player::equipSpecial(const Item::Special& specialItem)                                    {        pPrv->equipSpecial      ( specialItem                          ); }
const std::string Player::displayComponents(const PairList_t& components) const                { return pPrv->displayComponents ( components                           ); }