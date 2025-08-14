//CTask
#pragma once
#include<memory>
#include<string>
#include<vector>

#include"Debuff.h"
#include"Resources.h"
#include"Item.h"
#include"globals.h"

namespace Enemy {
    class Enemy;
}

typedef std::initializer_list<std::pair<std::string, uint16_t>> PairList_t;
typedef std::unique_ptr<Item::Item> ItemPtr_t;
typedef std::vector<ItemPtr_t> ItemPtrVec_t;

class PlayerPublic;
class PlayerPrivate;

class Player {
public:
    std::string_view name;
    uint16_t maxHealth,
             health,
             maxMana,
             mana,
             baseStrength,
             CON,
             level,
             bloodMeter;
    short strength, defense, 
    mageArmorDefense;
    uint16_t nextLevel, exp;
    Item::Weapon weapon;
    Item::Armor armor;
    Item::Special special;
    bool hasAbility,
         raceAbilityReady,
         classAbilityReady,
         arcaneEye,
         resurgence;
    std::vector<Debuff> debuffs;
    Resources resources;
    std::vector<std::string> knownWords;

    enum class RACE {
        ELF,
        HUMAN,
        DRAKONIAN,
        REVENANT
    };
    
    enum class CLASS {
        FIGHTER,
        ROGUE,
        WIZARD
    };
 
    void addExp(uint16_t);
    void levelUp(uint16_t);
    uint16_t getMaxHealth() const;
    short getStrength() const;
    short getDefense() const;
    void receiveGift();
    bool rituals();
    bool abilities(Enemy::Enemy* = nullptr, short* = nullptr, bool* = nullptr);
    uint16_t heal(float = 2);
    uint16_t healMax();
    void addDebuff(Debuff::TYPE);
    void removeDebuff(Debuff::TYPE);
    void updateDebuffs();
    void displayDebuffs() const;
    void initGeneral(const Item::General&, const Item::Source);
    void initGeneral(const Item::TYPE, const Item::Source);
    void initArmor(const Item::Armor&, const Item::Source);
    void initArmor(const Item::TYPE, const Item::Source, const uint16_t = 1);
    void initWeapon(const Item::Weapon&, const Item::Source);
    void initWeapon(const Item::TYPE, const Item::Source, const uint16_t = 1);
    void initSpecial(const Item::Special&, const Item::Source);
    void initSpecial(const Item::TYPE, const Item::Source);
    void unequipArmor(const bool = true);
    void unequipWeapon(const bool = true);
    void gatherResources();
    void craft();
    
private:
    std::unique_ptr<PlayerPublic>  pPub;
    std::unique_ptr<PlayerPrivate> pPrv;
    
    const struct Race_t {
        RACE Race;
        std::string_view name;
        bool operator==(const RACE compRace) const {
            return Race == compRace;
        }
        bool operator!=(const RACE compRace) const {
            return Race != compRace;
        }
    };

    inline static constexpr Race_t RaceInfo[] {
        { RACE::ELF,       "Elf"       },
        { RACE::HUMAN,     "Human"     },
        { RACE::DRAKONIAN, "Drakonian" },
        { RACE::REVENANT,  "Revenant"  }
    };
    
    const struct Class_t {
        CLASS Class;
        std::string_view name;
        bool operator==(const CLASS compClass) const {
            return Class == compClass;
        }
        bool operator!=(const CLASS compClass) const {
            return Class != compClass;
        }
    };
    
    inline static constexpr Class_t ClassInfo[] {
        { CLASS::FIGHTER, "Fighter" },
        { CLASS::ROGUE,   "Rogue"   },
        { CLASS::WIZARD,  "Wizard"  }
    };

    bool initCraftArmor(ItemPtrVec_t& items, uint16_t& i) const;
    bool initCraftWeapons(ItemPtrVec_t& items, uint16_t& i) const;
    bool initCraftGeneral(ItemPtrVec_t& items, uint16_t& i) const;
    bool useComponents(const PairList_t& components);
    bool craftItem(const ItemPtr_t& item, const PairList_t& components);
    void equipArmor(const Item::Armor& armorItem);
    void equipWeapon(const Item::Weapon& weaponItem);
    void equipSpecial(const Item::Special& specialItem);
    const std::string displayComponents(const PairList_t& components) const;

public:

    Race_t Race;
    Class_t Class;

    Player(std::string_view, RACE, CLASS, uint16_t, uint16_t, uint16_t);
    ~Player();

    void displayStats(const bool = false) const;

};