//CTask
#include<initializer_list>
#include<iostream>
#include<memory>
#include<string>
#include<vector>
#include<cmath>

#include"TextManager.h"
#include"Choice.h"
#include"Player.h"
#include"Enemy.h"
#include"Item.h"
#include"util.h"

Player::Player(std::string_view n, RACE ra, CLASS cl, uint16_t h, uint16_t s, uint16_t co) :
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
    setMode(LIST_OUT);
    std::cout << TM::get("hr");
    TM::print("player.stats", {
        .replacements = {
            {"{name}"             , (STR)name                                                                               },
            {"{race}"             , (STR)Race.name                                                                          },
            {"{blood_meter}"      , (Race == RACE::REVENANT ? "\nBlood Meter: " + TO_STR(bloodMeter) + "/3" : "")           },
            {"{class}"            , (STR)Class.name                                                                         },
            {"{mana}"             , (Class == CLASS::WIZARD ? "\nMana Points: " + TO_STR(mana) + '/' + TO_STR(maxMana) : "")},
            {"{health}"           , TO_STR(health)                                                                          },
            {"{max_health}"       , TO_STR(getMaxHealth())                                                                  },
            {"{str}"              , TO_STR(strength)                                                                        },
            {"{con}"              , TO_STR(CON)                                                                             },
            {"{def}"              , TO_STR(defense)                                                                         },
            {"{weapon}"           , weapon.name                                                                             },
            {
                "{weapon_data}",
                weapon.itemType != Item::TYPE::NONE
                ? " (Level: " + TO_STR(weapon.level) + ", Strength Bonus: " + TO_STR(weapon.strengthBonus) + ')'
                : ""
            },
            {"{armor}"            , armor.name                                                                              },
            {
                "{armor_data}",
                armor.itemType != Item::TYPE::NONE
                ? " (Level: " + TO_STR(armor.level) + ", Defense Bonus: " + TO_STR(armor.defenseBonus) + ')'
                : ""
            },
            {"{special}"          , special.name                                                                            },
            {
                "{special_data}",
                special.itemType != Item::TYPE::NONE
                ? " (Defense Bonus: " + TO_STR(special.defenseBonus) + ", Strength Bonus: " + TO_STR(special.strengthBonus) + ')'
                : ""
            },
            {"{level}"            , TO_STR(level)                                                                           },
            {"{exp}"              , TO_STR(exp)                                                                             },
            {"{next}"             , TO_STR(nextLevel)                                                                       }
        },
        .end = "\0"
    });
    std::cout << TM::get("hr");
    if (!debuffs.empty())
        displayDebuffs();
    if (showResources)
        resources.displayResources();
}

const std::string displayComps(const Resources& resources, const PairList_t& components) {
    std::string result = "Components:";
    for (const std::pair<std::string, uint16_t>& component : components) {
        if (resources.resources.count(component.first) != 0)
            result += "\n\t" + component.first + ": " + std::to_string(resources.resources.at(component.first)) + '/' + TO_STR(component.second);
        else
            result += "\n\t??";
    }
    return result;
}

bool initCraft(const Resources& resources, ItemPtrVec_t& items, uint16_t& i, const Item::TYPE type, const uint16_t level = 0, const short add = 1) {
    const Item::Info item = Item::Data[Item::toIdx(type)];
    if (item.type == Item::ItemClass::ARM) {
        items.emplace_back(std::make_unique<Item::Armor>(type, level, add, true));
        TM::print("player.craft.armor", {
            .replacements = {
                {"{num}", TO_STR(++i)},
                {"{type}", std::string(item.name)},
                {"{def}", TO_STR(Item::Armor::getStats(type, level))},
                {"{cmp}", displayComps(resources, item.components)}
            }
        });
    } else if (item.type == Item::ItemClass::WPN) {
        items.emplace_back(std::make_unique<Item::Weapon>(type, level, add, true));
        TM::print("player.craft.weapon", {
            .replacements = {
                {"{num}", TO_STR(++i)},
                {"{type}", std::string(item.name)},
                {"{str}", TO_STR(Item::Weapon::getStats(type, level))},
                {"{cmp}", displayComps(resources, item.components)}
            }
        });
    } else if (item.type == Item::ItemClass::GEN) {
        items.emplace_back(std::make_unique<Item::Item>(type));
        TM::print("player.craft.item", {
            .replacements = {
                {"{num}", TO_STR(++i)},
                {"{type}", std::string(item.name)},
                {"{cmp}", displayComps(resources, item.components)}
            }
        });
    }
    return true;
}

bool Player::initCraftArmor(ItemPtrVec_t& items, uint16_t& i) const {
    if (Class == Player::CLASS::WIZARD) {
        TM::print("player.craft.none", {
            .replacements = {
                {"{item}", "armor"}
            }
        });
        return false;
    }
    if (Race == Player::RACE::DRAKONIAN)
        initCraft(resources, items, i, Item::TYPE::ARM_DRAKONIAN, level, shrt(1));
    else {
        initCraft(resources, items, i, Item::TYPE::ARM_LEATHER, level, shrt(1));
        initCraft(resources, items, i, Item::TYPE::ARM_CHITIN, level, shrt(1));
        initCraft(resources, items, i, Item::TYPE::ARM_HIDE, level, shrt(1));
        initCraft(resources, items, i, Item::TYPE::ARM_SHAD_COWL, level, shrt(1));
    }
    return true;
}

bool Player::initCraftWeapons(ItemPtrVec_t& items, uint16_t& i) const {
    initCraft(resources, items, i, Item::TYPE::WPN_LONG, level, shrt(Class == Player::CLASS::WIZARD ? -1 : 1));
    initCraft(resources, items, i, Item::TYPE::WPN_BEAST_BLADE, level, shrt(Class == Player::CLASS::WIZARD ? -1 : 1));
    if (Class == Player::CLASS::WIZARD) {
        initCraft(resources, items, i, Item::TYPE::WPN_ST_WARBORN, level, shrt(Race == Player::RACE::ELF ? 2 : 1));
        initCraft(resources, items, i, Item::TYPE::WPN_ST_GUARDIAN, level, shrt(Race == Player::RACE::ELF ? 2 : 1));
        initCraft(resources, items, i, Item::TYPE::WPN_ST_SHADOW, level, shrt(Race == Player::RACE::ELF ? 2 : 1));
        initCraft(resources, items, i, Item::TYPE::WPN_ST_FURY, level, shrt(Race == Player::RACE::ELF ? 2 : 1));
        initCraft(resources, items, i, Item::TYPE::WPN_ST_WEEPING, level, shrt(Race == Player::RACE::ELF ? 2 : 1));
    }
    return true;
}

bool Player::initCraftGeneral(ItemPtrVec_t& items, uint16_t& i) const {
    initCraft(resources, items, i, Item::TYPE::POTION);
    if (Class == Player::CLASS::WIZARD)
        initCraft(resources, items, i, Item::TYPE::FOCUS);
    return true;
}

bool Player::useComponents(const PairList_t& components) {
    for (const std::pair<std::string, uint16_t>& component : components) {
        if (resources[component.first] < component.second) {
            TM::print("player.craft.no_res");
            return false;
        }
        resources[component.first] -= component.second;
    }
    return true;
}

bool Player::craftItem(const ItemPtr_t& item) {
    if (!useComponents(Item::Data[Item::toIdx(item->itemType)].components))
        return false;
    TM::print("player.craft.craft", {.replacements = {{"{item}", item->name}}});
    wheel();
    switch (Item::Data[ui16(item->itemType)].type) {
    case Item::ItemClass::ARM:
        initArmor(static_cast<const Item::Armor&>(*item), Item::Source::CRAFT);
        break;
    case Item::ItemClass::WPN:
        initWeapon(static_cast<const Item::Weapon&>(*item), Item::Source::CRAFT);
        if (!Item::Data[ui16(item->itemType)].canHavePrefix) // If the item is a staff
            equipSpecial(Item::Special()); // Remove the player's amulet
        break;
    default:
        initGeneral(static_cast<const Item::General&>(*item), Item::Source::CRAFT);
        break;
    }
    return true;
}

void Player::equipArmor(const Item::Armor& armorItem) {
    if (Race == Player::RACE::DRAKONIAN && armorItem != Item::TYPE::ARM_DRAKONIAN) {
        TM::print("player.equip.drakonian");
        return;
    }
    if (Class == Player::CLASS::WIZARD) {
        TM::print("player.equip.wizard");
        return;
    }
    if (armor != Item::TYPE::NONE)
        TM::print("player.equip.current.armor", {
            .replacements = {
                {"{item}", armor.name},
                {"{def}", TO_STR(armor.defenseBonus)}
            }
        });

    TM::print("player.equip.confirmation", {.replacements = {{"{item}", armorItem.name}}});
    int choice;
    do choice = Choice(input(prompt.data())).isChoice({"yes", "no"});
    while (choice == 0);

    if (choice == 2) {
        type("You choose to leave the " + armorItem.name + ".\n");
        return;
    }
    type("You equip the " + armorItem.name + ".\n");
    unequipArmor(false);
    defense += armorItem.defenseBonus;
    armor = armorItem;
}

void Player::equipWeapon(const Item::Weapon& weaponItem) {
    if (weapon != Item::TYPE::NONE)
        type("Your current " + weapon.name + " has (Strength Bonus: " + TO_STR(weapon.strengthBonus) + ")\n");

    type("\nDo you want to equip the " + weaponItem.name + " (1. Yes / 2. No)?\n");
    int choice;
    do choice = Choice(input(prompt.data())).isChoice({"yes", "no"});
    while (choice == 0);

    if (choice == 2) {
        type("You choose to leave the " + weaponItem.name + ".\n");
        return;
    }
    type("You equip the " + weaponItem.name + ".\n");
    unequipWeapon(false);
    strength += weaponItem.strengthBonus;
    weapon = weaponItem;
    if (weapon == Item::TYPE::WPN_ST_WARBORN) {
        maxMana += 3;
        mana += 3;
    } else if (weapon == Item::TYPE::WPN_ST_WEEPING) {
        maxMana -= 3;
        mana = ui16(std::max(mana - 3, 0));
    }
}

void Player::equipSpecial(const Item::Special& specialItem) {
    if (special != Item::TYPE::NONE && specialItem != Item::TYPE::NONE) {
        type("A brittle snap splits the air as your old " + special.name + " crumbles to dust.\n");
        resources[special.name] = 0;
    }
    defense -= special.defenseBonus;
    strength -= special.strengthBonus;
    defense += specialItem.defenseBonus;
    strength += specialItem.strengthBonus;
    special = specialItem;
}

const std::string Player::displayComponents(const PairList_t& components) const {
    return displayComps(resources, components);
}

void Player::addExp(uint16_t exp) {
    this->exp += exp;
    if (armor != Item::TYPE::NONE)
        armor.exp += exp;
    if (weapon != Item::TYPE::NONE)
        weapon.exp += exp;
}

void Player::levelUp(uint16_t hitdie) {
    uint16_t levels = 0;
    while (exp >= nextLevel) {
        level++;
        exp -= nextLevel;
        maxHealth += level + hitdie;
        health = getMaxHealth();
        strength += 3;
        baseStrength += 3;
        if (Race == Player::RACE::DRAKONIAN)
            defense += 1;
        nextLevel = 10 + ui16(pow(level, 2));
        levels++;
    }
    if (levels > 0) {
        type("\nYou leveled up");
        if (levels == 2)
            type(" twice");
        else if (levels == 3)
            type(" three times");
        else if (levels > 3)
            type(" many times");
        type("! You are now level " + TO_STR(level) + ".\n");
    }
    defense += armor.levelUp();
    strength += weapon.levelUp();
}

uint16_t Player::getMaxHealth() const {
    uint16_t maxHealth = this->maxHealth;
    for (const Debuff& debuff : debuffs) {
        if (debuff.hMod != 0)
            maxHealth = ui16(maxHealth * (1 + debuff.hMod));
    }
    return maxHealth;
}

short Player::getStrength() const {
    short strength = this->strength;
    for (const Debuff& debuff : debuffs) {
        if (debuff.strMod != 0)
            strength = ui16(strength * (1 + debuff.strMod));
    }
    return strength;
}

short Player::getDefense() const {
    short defense = this->defense;
    for (const Debuff& debuff : debuffs) {
        if (debuff.defMod != 0)
            defense = ui16(defense * (1 + debuff.defMod));
    }
    return defense;
}

void Player::receiveGift() {
    resources.addResource("Wood", randint(1, 3))
    ->addResource("Stone", randint(1, 3))
    ->addResource("Fiber", randint(1, 3))
    ->addResource("Leather", randint(1, 3));
    exp += randint(5, 15);
}

bool Player::rituals() {
    if (!classAbilityReady) {
        type("\nYou cannot perform any more rituals today.\n");
        return 0;
    }
    std::vector<const char*> rituals;
    uint16_t ritualNum = 0;
    type (
        "\t" + TO_STR(++ritualNum) + ". Mana Restoration\n" +
        TO_STR(ritualNum + 1) + ". (go back)\n"
    );

    rituals.push_back("Mana Restoration");
    rituals.push_back("(go back)");

    int ritualChoice;
    do ritualChoice = Choice(input(prompt.data())).isChoice(rituals);
    while (ritualChoice == 0);

    if (streq(rituals.at(--ritualChoice), "(go back)"))
        return 1;
        
    if (resources["Arcane Focus"] < 1) {
        type("\nThis ritual requires an arcane focus.\n");
        return 0;
    }
    resources["Arcane Focus"]--;
    mana = std::min(ui16(mana + 4), maxMana);
    type (
        "\nYou channel the energy of your Arcane Focus."
        "\nAs it dissolves, its energy seeps into you, leaving you refreshed and ready to cast once more.\n"
    );
    classAbilityReady = false;
    return 1;
}

bool Player::abilities(Enemy::Enemy* enemy, short* mirrorImage, bool* shadowmeld) {
    if (!hasAbility && Class != Player::CLASS::WIZARD) {
        type("\nYou don't have any abilities.\n");
        return 0;
    }
    if (!raceAbilityReady && !classAbilityReady && mana == 0) {
        if (Race != Player::RACE::REVENANT || bloodMeter == 3) {
            type("\nYour abilities are currently unavailable.\nYou must sleep first.\n");
            return 0;
        }
    }
    std::string output = "\nAbilities:\n";
    std::vector<const char*> abilities;
    uint16_t abilityNum = 0;
    // Race abilities
    if (enemy != nullptr && Race == Player::RACE::DRAKONIAN && raceAbilityReady) {
        abilities.push_back("Dragon's Breath");
        output += "\t" + TO_STR(++abilityNum) + ". Dragon's Breath (Drakonian)\n";
    } else if (enemy != nullptr && Race == Player::RACE::REVENANT) {
        if (raceAbilityReady) {
            abilities.push_back("Shadowmeld");
            output += "\t" + TO_STR(++abilityNum) + ". Shadowmeld (Revenant)\n";
        }
        if (bloodMeter < 3) {
            abilities.push_back("Necrotic Drain");
            output += "\t" + TO_STR(++abilityNum) + ". Necrotic Drain " + TO_STR(bloodMeter) + "/3 (Revenant)\n";
        }
    }
    // Class abilities
    if (Class == Player::CLASS::FIGHTER && classAbilityReady) {
        abilities.push_back("Second Wind");
        output += "\t" + TO_STR(++abilityNum) + ". Second Wind (Fighter)\n";
    }
    if (Class == Player::CLASS::WIZARD) {
        output += "Spells (You have " + TO_STR(mana) + " mana points):\n";
        if (enemy != nullptr) {
            abilities.push_back("Fire Bolt");
            output += "\t" + TO_STR(++abilityNum) + ". Fire Bolt (Wizard) - 1 MP\n";
            abilities.push_back("Mirror Image");
            output += "\t" + TO_STR(++abilityNum) + ". Mirror Image (Wizard) - 2 MP\n";
        }
        abilities.push_back("Mage Armor");
        output += "\t" + TO_STR(++abilityNum) + ". Mage Armor (Wizard) - 5 MP\n";
        if (enemy == nullptr) {
            abilities.push_back("Arcane Eye");
            output += "\t" + TO_STR(++abilityNum) + ". Arcane Eye (Wizard) - 3 MP\n";
        }
        if (weapon == Item::TYPE::WPN_ST_GUARDIAN) {
            abilities.push_back("Recovery");
            output += "\t" + TO_STR(++abilityNum) + ". Recovery (Staff of the Guardian) - 2 MP\n";
        }
    }
    abilities.push_back("(go back)");
    output += TO_STR(abilityNum + 1) + ". (go back)\n";
    if (abilities.size() == 0) {
        type("\nYour abilities are currently unavailable.\n");
        return 0;
    }
    type((STR)output);

    int abilityChoice;
    do abilityChoice = Choice(input(prompt.data())).isChoice(abilities);
    while (abilityChoice == 0);

    if (streq(abilities.at(--abilityChoice), "Dragon's Breath")) {
        const uint16_t damage = strength + randint(1, 6);
        const uint16_t burn = randint(1, 4);
        enemy->health = ui16(std::max(enemy->health - damage - burn, 0));
        type (
            "\nYou take a deep breath, and with a powerful exhale, a torrent of searing flames erupts from your mouth, searing the " + (STR)enemy->name + " for " + TO_STR(damage) + " damage!"
            "\nThe " + (STR)enemy->name + " is burned for an additional " + TO_STR(burn) + " damage!"
            "\nIts health is now " + TO_STR(enemy->health) + ".\n"
        );
        raceAbilityReady = false;
    } else if (streq(abilities.at(abilityChoice), "Shadowmeld")) {
        type (
            "\nYou meld into the shadows, becoming nearly invisible."
            "\nThe enemy can't see you!\n"
        );
        *shadowmeld = true;
        raceAbilityReady = false;
        return 0;
    } else if (streq(abilities.at(abilityChoice), "Necrotic Drain")) {
        const uint16_t damage = strength / 2 + randint(1, 6);
        enemy->health = ui16(std::max(enemy->health - damage, 0));
        health = std::min(ui16(health + damage / 2), getMaxHealth());
        bloodMeter++;
        type (
            "\nYou drain the life force from the " + (STR)enemy->name + ", dealing " + TO_STR(damage) + " damage!"
            "\nIts health is now " + TO_STR(enemy->health) + "."
            "\nYou also fill your blood meter and gain " + TO_STR(damage / 2) + " health points!"
            "\nYour health is now " + TO_STR(health) + "."
            "\nYour blood meter is now at " + TO_STR(bloodMeter) + "/3.\n"
        );
    } else if (streq(abilities.at(abilityChoice), "Second Wind")) {
        const uint16_t healing = heal();
        type (
            "\nYou get a surge of adrenaline and heal " + TO_STR(healing) + " points!"
            "\nYour health is now " + TO_STR(health) + ".\n"
        );
        classAbilityReady = false;
        return 0;
    } else if (streq(abilities.at(abilityChoice), "Fire Bolt")) {
        if (mana < 1) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        const uint16_t damage = strength + randint(1, 8);
        const uint16_t burn = randint(1, 4);
        enemy->health = ui16(std::max(enemy->health - damage - burn, 0));
        mana--;
        health = std::min(ui16(health + 1), getMaxHealth());
        type (
            "\nYou conjure a blazing ember in your palm and hurl it forward."
            "\nThe fire bolt streaks through the air, striking the " + (STR)enemy->name + " with a burst of flames for " + TO_STR(damage) + " damage!"
            "\nThe " + (STR)enemy->name + " is burned for an additional " + TO_STR(burn) + " damage!"
            "\nIts health is now " + TO_STR(enemy->health) + ".\n"
        );
    } else if (streq(abilities.at(abilityChoice), "Mirror Image")) {
        if (mana < 2) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        *mirrorImage = 2;
        mana -= 2;
        health = std::min(ui16(health + 2), getMaxHealth());
        type (
            "\nYou weave an illusion, creating shimmering duplicates of yourself."
            "\nThey flicker and shift, making it difficult for the enemy to land its strikes.\n"
        );
        return 0;
    } else if (streq(abilities.at(abilityChoice), "Mage Armor")) {
        if (mageArmorDefense > 0) {
            type("\nThis spell is already active!\n");
            return 0;
        }
        if (mana < 5) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        mageArmorDefense = ui16(pow(level + 1, 1.2f));
        defense += mageArmorDefense;
        mana -= 5;
        health = std::min(ui16(health + 5), getMaxHealth());
        type("\nA protective shielding aura surrounds you, boosting your defense by " + TO_STR(mageArmorDefense) + "!\n");
    } else if (streq(abilities.at(abilityChoice), "Arcane Eye")) {
        if (mana < 3) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        arcaneEye = true;
        mana -= 3;
        health = std::min(ui16(health + 3), getMaxHealth());
        type (
            "\nYou materialize your magic, forming an invisible, floating eye."
            "\nIt scouts ahead, ensuring you are never caught off guard.\n"
        );
    } else if (streq(abilities.at(abilityChoice), "Recovery")) {
        if (mana < 2) {
            type("\nYou don't have enough mana points!\n");
            return 0;
        }
        const uint16_t healing = heal(3);
        mana -= 2;
        health = std::min(ui16(health + 2), getMaxHealth());
        type (
            "\nYou channel magical energy into a healing aura, wrapping yourself in a warm, sooting light."
            "\nYour wounds are mended and you heal " + TO_STR(healing) + " points!"
            "\nYour health is now " + TO_STR(health) + ".\n"
        );
        return 0;
    } else
        return 0;
    return 1;
}

uint16_t Player::heal(float div) {
    static const float HEALING_MULTIPLIER = randint(10, 15) / 10.f;
    if (div < HEALING_MULTIPLIER)
        div = HEALING_MULTIPLIER;
    uint16_t healing = ui16((getMaxHealth() - health) * HEALING_MULTIPLIER / div / 1.5);
    /*
    function: f(h) = r(m - h) / 2 where
    f(h) = healing
    m = max health
    h = current health
    r = random number between 1.0 and 1.5
    */
    if (Race == Player::RACE::REVENANT)
        healing /= 2;
    health += healing;
    return healing;
}

uint16_t Player::healMax() {
    if (Race == Player::RACE::REVENANT)
        health += (getMaxHealth() - health) / 2;
    else
        health = getMaxHealth();
    return health;
}

void Player::addDebuff(Debuff::TYPE debuffType) {
    debuffs.push_back(Debuff(debuffType));
    health = std::min(health, getMaxHealth());
    const Debuff::Info& debuff = Debuff::Data[debuffType];
    type("You are now " + (STR)debuff.name + ".\n");

    std::vector<std::string> effects;

    auto addEffect = [&](const char* stat, float mod) {
        if (mod < 0)
            effects.push_back((STR)stat + " is decreased by " + TO_STR((uint16_t)(std::abs(mod * 100))) + '%');
        else if (mod > 0)
            effects.push_back((STR)stat + " is increased by " + TO_STR((uint16_t)(std::abs(mod * 100))) + '%');
    };

    addEffect("health", debuff.hMod);
    addEffect("strength", debuff.strMod);
    addEffect("defense", debuff.defMod);

    if (!effects.empty()) {
        type("Your ");
        for (size_t i = 0; i < effects.size(); i++) {
            type((STR)effects.at(i));
            if (i + 1 < effects.size())
                type(", ");
        }
    }
    type(" for " + (debuff.duration < UINT16_MAX ? TO_STR(debuff.duration) : "an indefinite amount of") + " turns.\n");
}

void Player::removeDebuff(Debuff::TYPE debuffType) {
    for (std::vector<Debuff>::iterator it = debuffs.begin(); it != debuffs.end(); it++) {
        if (it->name == Debuff::Data[debuffType].name) {
            type("\nYou are no longer " + (STR)it->name + ".\n");
            debuffs.erase(it);
            return;
        }
    }
}

void Player::updateDebuffs() {
    for (std::vector<Debuff>::iterator it = debuffs.begin(); it != debuffs.end(); ) {
        if (it->duration == 1) {
            type("\nYou are no longer " + (STR)it->name + ".\n");
            it = debuffs.erase(it);
        } else {
            if (it->duration < UINT16_MAX)
                it->duration--;
            it++;
        }
    }
}

void Player::displayDebuffs() const {
    type("\nDebuffs:\n");
    if (debuffs.empty()) {
        setMode(LIST_OUT);
        type("\tNone\n");
        return;
    }

    for (const Debuff& debuff : debuffs) {
        type("\t" + (STR)debuff.name + " (");

        std::vector<std::string> effects;

        auto addEffect = [&](const char* stat, float mod, short base) {
            if (mod < 0)
                effects.push_back("decreased " + (STR)stat + " by " + TO_STR((short)(std::round(base - base * (1 + mod)))));
            else if (mod > 0)
                effects.push_back("increased " + (STR)stat + " by " + TO_STR((short)(std::round(base - base * (1 + mod)))));
        };
        
        addEffect("health", debuff.hMod, maxHealth);
        addEffect("strength", debuff.strMod, strength);
        addEffect("defense", debuff.defMod, defense);

        for (size_t i = 0; i < effects.size(); i++) {
            type((STR)effects.at(i));
            if (i + 1 < effects.size())
                type(", ");
        }
        type(" for " + (debuff.duration < UINT16_MAX ? TO_STR(debuff.duration) : "an indefinite amount of") + " turns)\n");
    }
}

void Player::initArmor(const Item::Armor& armor, const Item::Source source) {
    armor.displayInfo(source);
    equipArmor(armor);
}

void Player::initArmor(const Item::TYPE aType, const Item::Source source, const uint16_t add) {
    Item::Armor armor(aType, level, add);
    initArmor(armor, source);
}

void Player::initWeapon(const Item::Weapon& weapon, const Item::Source source) {
    weapon.displayInfo(source);
    equipWeapon(weapon);
}

void Player::initWeapon(const Item::TYPE wType, const Item::Source source, const uint16_t add) {
    Item::Weapon weapon(wType, level, add);
    initWeapon(weapon, source);
}

void Player::initSpecial(const Item::Special& special, const Item::Source source) {
    special.displayInfo(source);
    equipSpecial(special);
}

void Player::initSpecial(const Item::TYPE sType, const Item::Source source) {
    Item::Special special(sType, level);
    initSpecial(special, source);
}

void Player::initGeneral(const Item::General& general, const Item::Source source) {
    general.displayInfo(source);
    resources.addResource(general.name);
}

void Player::initGeneral(const Item::TYPE iType, const Item::Source source) {
    Item::General general(iType);
    initGeneral(general, source);
}

void Player::unequipArmor(const bool confirmation) {
    if (confirmation) {
        type("Are you sure you want to unequip your " + armor.name + " (1. Yes / 2. No)?\n");
        int choice;
        do choice = Choice(input(prompt.data())).isChoice({"yes", "no"});
        while (choice == 0);
        if (choice == 2)
            return;
        type("You unequip your " + armor.name + ".\n");
    }
    defense -= armor.defenseBonus;
    armor = Item::Armor();
}

void Player::unequipWeapon(const bool confirmation) {
    if (confirmation) {
        type("Are you sure you want to unequip your " + weapon.name + " (1. Yes / 2. No)?\n");
        int choice;
        do choice = Choice(input(prompt.data())).isChoice({"yes", "no"});
        while (choice == 0);
        if (choice == 2)
            return;
        type("You unequip your " + weapon.name + ".\n");
    }
    if (weapon == Item::TYPE::WPN_ST_WARBORN) {
        maxMana -= 3;
        mana = ui16(std::max(mana - 3, 0));
    } else if (weapon == Item::TYPE::WPN_ST_WEEPING) {
        maxMana += 3;
        mana += 3;
    }
    strength -= weapon.strengthBonus;
    weapon = Item::Weapon();
}

void Player::gatherResources() {
    type("\nCollecting resources...\n");
    wheel();

    Resources colResources;
    colResources.addResource("Wood", randint(0, level))
    ->addResource("Stone", randint(0, level))
    ->addResource("Fiber", randint(0, level))
    ->addResource("Leather", randint(0, level));

    resources.addResources(colResources);

    type("Done! Collected resources:\n");
    colResources.displayResources(false);
}

void Player::craft() {
    do {
        ItemPtrVec_t items;
        uint16_t i = 0;
        type (
            "\nWhat do you want to craft?"
            "\n1. Armor"
            "\n2. Weapons"
            "\n3. Items"
            "\n4. (go back)\n"
        );
        int typeChoice;
        do typeChoice = Choice(input(prompt.data())).isChoice({"armor", "weapons", "items", "(go back)"});
        while (typeChoice == 0);

        if (typeChoice == 4)
            return;

        std::cout << '\n';

        // Get what the player wants to craft and fill the items list with relevant items
        switch (typeChoice) {
            case 1: if (!initCraftArmor(items, i)) continue; break;
            case 2: if (!initCraftWeapons(items, i)) continue; break;
            case 3: if (!initCraftGeneral(items, i)) continue; break;
        }
        type(TO_STR(i + 1) + ". (go back)\n");

        while (true) {
            Choice craftChoice;
            uint16_t choiceNum = i;
            bool isValidChoice = false;
            do {
                craftChoice = input(prompt.data());
                for (uint16_t j = 0; j < i && !isValidChoice; j++) {
                    isValidChoice = craftChoice.isChoice(items[j]->name.c_str(), j + 1);
                    if (isValidChoice)
                        choiceNum = j;
                }
            } while (!(isValidChoice || craftChoice.isChoice("(go back)", i + 1, true)));

            if (choiceNum == i)
                break;

            craftItem(items.at(choiceNum));
            return;
        }
    } while (true);
}
