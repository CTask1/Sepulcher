#include"..\include\Player.h"
#include"..\include\PlayerPublic.h"
#include"..\include\PlayerPrivate.h"

Player::Player(std::string n, RACE ra, CLASS cl, int h, int s, int co) :
  pPub(std::make_unique<PlayerPublic>(*this)), pPrv(std::make_unique<PlayerPrivate>(*this)), name(n), Race({R[ra].Race, R[ra].name}), Class({C[cl].Class, C[cl].name}),
  maxHealth(h), health(h), maxMana(0), mana(0), baseStrength(s), strength(s), CON(co), defense(0),
  mageArmorDefense(0), level(1), nextLevel(10 + static_cast<uint32_t>(pow(level, 2))), exp(0),
  hasAbility(false), raceAbilityReady(false), classAbilityReady(false), arcaneEye(false) {
    if (Race == DRAKONIAN) {
        hasAbility = true;
        raceAbilityReady = true;
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

Player::~Player() {}

void Player::displayStats() const {
    std::cout << "\n-------------------------";
    setOutputSettings(true);
    type (
        "\nName: ", name,
        "\nRace: ", Race.name,
        "\nClass: ", Class.name,
        (Class == WIZARD ? "\nMana Points: " + std::to_string(mana) + '/' + std::to_string(maxMana) : ""),
        "\nHealth: ", health, "/", maxHealth,
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
}

void Player::receiveGift()                                                                                                             {        (*pPub).receiveGift       (                           ); }
const bool Player::rituals()                                                                                                           { return (*pPub).rituals           (                           ); }
const bool Player::abilities(Enemy::Enemy* enemy, short* mirrorImage)                                                                  { return (*pPub).abilities         ( enemy   , mirrorImage     ); }
const uint16_t Player::heal(const uint16_t div)                                                                                        { return (*pPub).heal              ( div                       ); }
void Player::initItem(const Item::Item& item, const Item::Source source)                                                               {        (*pPub).initItem          ( item    , source          ); }
void Player::initItem(const Item::TYPE iType, const Item::Source source)                                                               {        (*pPub).initItem          ( iType   , source          ); }
void Player::initArmor(const Item::Armor& armor, const Item::Source source)                                                            {        (*pPub).initArmor         ( armor   , source          ); }
void Player::initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add)                                          {        (*pPub).initArmor         ( aType   , source, add     ); }
void Player::initWeapon(const Item::Weapon& weapon, const Item::Source source)                                                         {        (*pPub).initWeapon        ( weapon  , source          ); }
void Player::initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add)                                         {        (*pPub).initWeapon        ( wType   , source, add     ); }
void Player::initSpecial(const Item::Special& special, const Item::Source source)                                                      {        (*pPub).initSpecial       ( special , source          ); }
void Player::initSpecial(const Item::TYPE sType, const Item::Source source)                                                            {        (*pPub).initSpecial       ( sType   , source          ); }
void Player::unequipArmor(const bool confirmation)                                                                                     {        (*pPub).unequipArmor      ( confirmation              ); }
void Player::unequipWeapon(const bool confirmation)                                                                                    {        (*pPub).unequipWeapon     ( confirmation              ); }
void Player::gatherResources()                                                                                                         {        (*pPub).gatherResources   (                           ); }
void Player::craft()                                                                                                                   {        (*pPub).craft             (                           ); }
const bool Player::initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i)                       { return (*pPrv).initCraftArmor    ( items   , i               ); }
const bool Player::initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i)                     { return (*pPrv).initCraftWeapons  ( items   , i               ); }
const bool Player::initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i)                       { return (*pPrv).initCraftItems    ( items   , i               ); }
const bool Player::checkComponents() const                                                                                             { return (*pPrv).checkComponents   (                           ); }
template<typename... Args> const bool Player::checkComponents(const std::string& resource, const uint16_t amount, const Args&... args) { return (*pPrv).checkComponents   ( resource, amount, args... ); }
void Player::useComponents()                                                                                                           {        (*pPrv).useComponents     (                           ); }
template<typename... Args> void Player::useComponents(const std::string& resource, const uint16_t amount, const Args&... args)         {        (*pPrv).useComponents     ( resource, amount, args... ); }
template<typename... Args> const bool Player::initCraft(const Item::Item* const& item, const Args&... args)                            { return (*pPrv).initCraft         ( item    ,         args... ); }
template<typename... Args> const bool Player::craftArmor(Item::Armor& armor, const Args&... args)                                      { return (*pPrv).craftArmor        ( armor   ,         args... ); }
template<typename... Args> const bool Player::craftWeapon(const Item::Weapon& weapon, const Args&... args)                             { return (*pPrv).craftWeapon       ( weapon  ,         args... ); }
template<typename... Args> const bool Player::craftItem(const Item::Item& item, const Args&... args)                                   { return (*pPrv).craftItem         ( item    ,         args... ); }
void Player::equipArmor(const Item::Armor& armorItem)                                                                                  {        (*pPrv).equipArmor        ( armorItem                 ); }
void Player::equipWeapon(const Item::Weapon& weaponItem)                                                                               {        (*pPrv).equipWeapon       ( weaponItem                ); }
void Player::equipSpecial(const Item::Special& specialItem)                                                                            {        (*pPrv).equipSpecial      ( specialItem               ); }
const std::string Player::getComponents() const                                                                                        { return (*pPrv).getComponents     (                           ); }
template<typename... Args> const std::string Player::getComponents(const std::string& name, const uint16_t& amt, const Args&... args)  { return (*pPrv).getComponents     ( name    , amt   , args... ); }
template<typename... Args> const std::string Player::displayComponents(const Args&... args)                                            { return (*pPrv).displayComponents ( args...                   ); } 