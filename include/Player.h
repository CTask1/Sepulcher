#pragma once
#include<initializer_list>
#include<unordered_map>
#include<string_view>
#include<vector>
#include<memory>

#include"Resources.h"
#include"Enemy.h"
#include"Item.h"
#include"consts.h"

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

    struct Info {
        const std::string_view name = "None";
        const uint16_t duration = 1;
        const float hMod   = 0.f,
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
    uint32_t nextLevel, exp;
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
 
    uint16_t getMaxHealth() const;
    short getStrength() const;
    short getDefense() const;
    void receiveGift();
    bool rituals();
    bool abilities(Enemy::Enemy* = nullptr, short* = nullptr, bool* = nullptr);
    uint16_t heal(const uint16_t = 2);
    uint16_t healMax();
    void addDebuff(Debuff::TYPE);
    void updateDebuffs();
    void displayDebuffs() const;
    void initItem(const Item::Item&, const Item::Source);
    void initItem(const Item::TYPE, const Item::Source);
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

    bool initCraftArmor(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const;
    bool initCraftWeapons(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const;
    bool initCraftItems(std::unordered_map<uint16_t, std::unique_ptr<Item::Item>>& items, uint16_t& i) const;
    bool useComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components);
    bool initCraft(const Item::Item* const& item, const std::initializer_list<std::pair<std::string, uint16_t>> components);
    bool craftArmor(Item::Armor& armor, const std::initializer_list<std::pair<std::string, uint16_t>> components);
    bool craftWeapon(const Item::Weapon& weapon, const std::initializer_list<std::pair<std::string, uint16_t>> componentss);
    bool craftItem(const Item::Item& item, const std::initializer_list<std::pair<std::string, uint16_t>> components);
    void equipArmor(const Item::Armor& armorItem);
    void equipWeapon(const Item::Weapon& weaponItem);
    void equipSpecial(const Item::Special& specialItem);
    const std::string displayComponents(const std::initializer_list<std::pair<std::string, uint16_t>> components) const;

public:

    RaceInfo Race;
    ClassInfo Class;

    Player(std::string_view, RACE, CLASS, uint16_t, uint16_t, uint16_t);
    ~Player();

    void displayStats() const;

};