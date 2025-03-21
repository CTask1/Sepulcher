#pragma once
#include"Resources.h"
#include"Enemy.h"
#include"Item.h"

class PlayerPublic;
class PlayerPrivate;

class Player {
public:
    std::string name;
    uint16_t maxHealth,
             health,
             maxMana,
             mana,
             baseStrength,
             CON,
             level;
    short strength, defense, 
    mageArmorDefense;
    uint32_t nextLevel, exp;
    Item::Weapon weapon;
    Item::Armor armor;
    Item::Special special;
    bool hasAbility,
         raceAbilityReady,
         classAbilityReady,
         arcaneEye;
    Resources resources;

    enum RACE {
        ELF,
        HUMAN,
        DRAKONIAN,
        REVENANT
    };
    
    enum CLASS {
        FIGHTER,
        ROGUE,
        WIZARD
    };
 
    void receiveGift();
    const bool rituals();
    const bool abilities(Enemy::Enemy* enemy = nullptr, short* mirrorImage = nullptr);
    const uint16_t heal(const uint16_t div = 2);
    void initItem(const Item::Item& item, const Item::Source source);
    void initItem(const Item::TYPE iType, const Item::Source source);
    void initArmor(const Item::Armor& armor, const Item::Source source);
    void initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add = 1);
    void initWeapon(const Item::Weapon& weapon, const Item::Source source);
    void initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add = 1);
    void initSpecial(const Item::Special& special, const Item::Source source);
    void initSpecial(const Item::TYPE sType, const Item::Source source);
    void unequipArmor(const bool confirmation = true);
    void unequipWeapon(const bool confirmation = true);
    void gatherResources();
    void craft();
    
private:
    std::unique_ptr<PlayerPublic>  pPub;
    std::unique_ptr<PlayerPrivate> pPrv;
    
    struct RaceInfo {
        const RACE Race;
        const std::string_view name;
        const bool operator==(const RACE compRace) const {
            return Race == compRace;
        }
        const bool operator!=(const RACE compRace) const {
            return Race != compRace;
        }
    };

    inline static constexpr RaceInfo R[] {
        { ELF,       "Elf"       },
        { HUMAN,     "Human"     },
        { DRAKONIAN, "Drakonian" },
        { REVENANT,  "Revenant"  }
    };
    
    struct ClassInfo {
        const CLASS Class;
        const std::string_view name;
        const bool operator==(const CLASS compClass) const {
            return Class == compClass;
        }
        const bool operator!=(const CLASS compClass) const {
            return Class != compClass;
        }
    };
    
    inline static constexpr ClassInfo C[] {
        { FIGHTER, "Fighter" },
        { ROGUE,   "Rogue"   },
        { WIZARD,  "Wizard"  }
    };

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

public:

    RaceInfo Race;
    ClassInfo Class;

    Player(std::string, RACE, CLASS, int, int, int);
    ~Player();

    void displayStats() const;

};