//CTask
#pragma once
#include<initializer_list>
#include<string_view>
#include<vector>
#include<memory>

#include"Resources.h"
#include"Enemy.h"
#include"Item.h"
#include"globals.h"

typedef std::initializer_list<std::pair<std::string, uint16_t>> PairList_t;
typedef std::unique_ptr<Item::Item> ItemPtr_t;
typedef std::vector<ItemPtr_t> ItemPtrVec_t;

class PlayerPublic;
class PlayerPrivate;

class Debuff {
public:
    std::string_view name;
    uint16_t duration;
    float hMod, strMod, defMod;

    enum TYPE {
        NONE,
        RAVENOUS
    };

    const struct Info {
        std::string_view name = "None";
        uint16_t duration = 1;
        float hMod   = 0.f,
              strMod = 0.f,
              defMod = 0.f;
    };

    static constexpr Info Data[] {
        { "None"                        },
        { "Ravenous", DAY_LENGTH, -0.1f }
    };

    Debuff(TYPE debuffType) :
        name     (Data[debuffType].name    ),
        duration (Data[debuffType].duration),
        hMod     (Data[debuffType].hMod    ),
        strMod   (Data[debuffType].strMod  ),
        defMod   (Data[debuffType].defMod  ) {}

};

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
    
    const struct RaceInfo {
        RACE Race;
        std::string_view name;
        bool operator==(const RACE compRace) const {
            return Race == compRace;
        }
        bool operator!=(const RACE compRace) const {
            return Race != compRace;
        }
    };

    inline static constexpr RaceInfo R[] {
        { ELF,       "Elf"       },
        { HUMAN,     "Human"     },
        { DRAKONIAN, "Drakonian" },
        { REVENANT,  "Revenant"  }
    };
    
    const struct ClassInfo {
        CLASS Class;
        std::string_view name;
        bool operator==(const CLASS compClass) const {
            return Class == compClass;
        }
        bool operator!=(const CLASS compClass) const {
            return Class != compClass;
        }
    };
    
    inline static constexpr ClassInfo C[] {
        { FIGHTER, "Fighter" },
        { ROGUE,   "Rogue"   },
        { WIZARD,  "Wizard"  }
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

    RaceInfo Race;
    ClassInfo Class;

    Player(std::string_view, RACE, CLASS, uint16_t, uint16_t, uint16_t);
    ~Player();

    void displayStats(const bool = false) const;

};