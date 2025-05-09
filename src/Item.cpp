//CTask
#include"Item.h"
#include"util.h"

namespace Item {

    // ---------- ITEM ---------- //
    Item::Item(const TYPE type) : itemType(type), name(Data[ui16(type)].name) {}
    Item::~Item() = default;
    
    void Item::displayInfo(const Source source) const {
        if (source == Source::FIND)
            type("\nYou found an item: ");
        else if (source == Source::CRAFT)
            type("\nYou crafted an item: ");
        else if (source == Source::DROP)
            type("\nThe enemy dropped an item: ");
        type(name);
    }
    
    bool Item::operator!=(const TYPE Type) const {
        return itemType != Type;
    }
    
    bool Item::operator==(const TYPE Type) const {
        return itemType == Type;
    }

    // ---------- GENERAL ---------- //
    General::General(const TYPE t) : Item(t) {}
    
    void General::displayInfo(const Source source) const {
        Item::displayInfo(source);
        type("\n");
    }
    
    // ---------- LEVELED ---------- //
    Leveled::Leveled(const TYPE t) : Item(t), exp(0), level(1), nextLevel(11) {}
    
    uint16_t Leveled::levelUp() {
        level++;
        exp -= nextLevel;
        nextLevel = 10 + ui16(pow(level, 2)); // 10 + level to the power of 2
        return 0;
    }
    
    // ---------- ARMOR ---------- //
    short Armor::getStats(const TYPE armType, const uint16_t level, const short add) {
        return shrt(std::round(pow(level + add, Data[ui16(armType)].defMod)));
    }

    Armor::Armor() : defenseBonus(0) {}
    
    Armor::Armor(const TYPE armType, const uint16_t level, const short add, const bool crafted) :
        Leveled(armType),
        defenseBonus (crafted // if the item is crafted
            ? shrt(std::round(pow(level + add, Data[ui16(armType)].defMod))) // (level + add) to the power of defMod
            : shrt(randint(1, ui16(std::round(pow(level + add, Data[ui16(armType)].defMod))))) // random number between 1 and the value above
        ),
        prefix(Prefix::NONE),
        suffix(Suffix::NONE) {
            // Set prefix
            uint16_t num = randint(1, 100);
            uint16_t prb = ui16(Prefix::NONE);
            if (num > prb && num <= prb + ui16(Prefix::RUSTED)) {
                name = "Rusted " + name;
                prefix = Prefix::RUSTED;
                defenseBonus = shrt(defenseBonus * 0.8f);
            }
            prb += ui16(Prefix::RUSTED);
            if (num > prb && num <= prb + ui16(Prefix::WORN)) {
                name = "Worn " + name;
                prefix = Prefix::WORN;
                defenseBonus = shrt(defenseBonus * 0.9f);
            }
            prb += ui16(Prefix::WORN);
            if (num > prb && num <= prb + ui16(Prefix::HEAVY)) {
                name = "Heavy " + name;
                prefix = Prefix::HEAVY;
                defenseBonus = shrt(defenseBonus * 1.1f);
            }
            prb += ui16(Prefix::HEAVY);
            if (num > prb && num <= prb + ui16(Prefix::ENCHANTED)) {
                name = "Enchanted " + name;
                prefix = Prefix::ENCHANTED;
                defenseBonus = shrt(defenseBonus * 1.2f);
            }

            // Set suffix
            num = randint(1, 100);
            prb = ui16(Suffix::NONE);
            if (num > prb && num <= prb + ui16(Suffix::THORNS)) {
                name += " of Thorns";
                suffix = Suffix::THORNS;
                return;
            }
            prb += ui16(Suffix::THORNS);
            if (num > prb && num <= prb + ui16(Suffix::KNIGHT)) {
                name += " of the Knight";
                suffix = Suffix::KNIGHT;
                defenseBonus = shrt(defenseBonus * 1.1f);
                return;
            }
            prb += ui16(Suffix::KNIGHT);
            if (num > prb && num <= prb + ui16(Suffix::FORTITUDE)) {
                name += " of Fortitude";
                suffix = Suffix::FORTITUDE;
                defenseBonus = shrt(defenseBonus * 1.2f);
                return;
            }
            prb += ui16(Suffix::FORTITUDE);
            if (num > prb && num <= prb + ui16(Suffix::RESILIENCE)) {
                name += " of Resilience";
                suffix = Suffix::RESILIENCE;
                defenseBonus = shrt(defenseBonus * 1.3f);
                return;
            }
        }

    uint16_t Armor::levelUp() {
        uint16_t bonus = 0;
        uint16_t levels = 0;
        while (exp >= nextLevel) {
            bonus += shrt(std::round(pow(level, Data[ui16(itemType)].defMod / 2))); // level to the power of defMod / 2
            Leveled::levelUp();
            levels++;
        }
        if (levels > 0) {
            type("\nYour ", name, " leveled up");
            if (levels == 2)
                type(" twice");
            else if (levels == 3)
                type(" three times");
            else if (levels > 3)
                type(" many times");
            type("! It is now level ", level, ".\n");
        }
        defenseBonus += bonus;
        return bonus;
    }
    
    void Armor::displayInfo(const Source source) const {
        Item::displayInfo(source);
        type(" (Defense Bonus: ", defenseBonus, ")\n");
    }

    // ---------- WEAPON ---------- //
    short Weapon::getStats(const TYPE wpnType, const uint16_t level, const short add) {
        return shrt(std::abs(std::round(pow(level + add, Data[ui16(wpnType)].strMod))));
    }

    Weapon::Weapon() : strengthBonus(0) {}
    
    Weapon::Weapon(const TYPE wpnType, const uint16_t level, const short add, const bool crafted) :
        Leveled(wpnType),
        strengthBonus (crafted // if the item is crafted
            ? shrt(std::abs(std::round(pow(level + add, Data[ui16(wpnType)].strMod)))) // (level + add) to the power of strMod
            : shrt(randint(1, ui16(std::abs(std::round(pow(level + add, Data[ui16(wpnType)].strMod)))))) // random number between 1 and the value above
        ),
        prefix(Prefix::NONE),
        suffix(Suffix::NONE) {
            uint16_t num;
            uint16_t prb;
            // Set prefix
            if (Data[ui16(wpnType)].canHavePrefix) {
                num = randint(1, 100);
                prb = ui16(Prefix::NONE);
                if (num > prb && num <= prb + ui16(Prefix::CURSED)) {
                    name = "Cursed " + name;
                    prefix = Prefix::CURSED;
                    strengthBonus = shrt(strengthBonus * 0.8f);
                }
                prb += ui16(Prefix::CURSED);
                if (num > prb && num <= prb + ui16(Prefix::DULL)) {
                    name = "Dull " + name;
                    prefix = Prefix::DULL;
                    strengthBonus = shrt(strengthBonus * 0.9f);
                }
                prb += ui16(Prefix::DULL);
                if (num > prb && num <= prb + ui16(Prefix::SHARP)) {
                    name = "Sharp " + name;
                    prefix = Prefix::SHARP;
                    strengthBonus = shrt(strengthBonus * 1.1f);
                }
                prb += ui16(Prefix::SHARP);
                if (num > prb && num <= prb + ui16(Prefix::LEGENDARY)) {
                    name = "Legendary " + name;
                    prefix = Prefix::LEGENDARY;
                    strengthBonus = shrt(strengthBonus * 1.2f);
                }
            }

            // Set suffix
            num = randint(1, 100);
            prb = ui16(Suffix::NONE);
            if (num > prb && num <= prb + ui16(Suffix::INFERNO)) {
                name += " of the Inferno";
                suffix = Suffix::INFERNO;
                return;
            }
            prb += ui16(Suffix::INFERNO);
            if (num > prb && num <= prb + ui16(Suffix::VENGEANCE)) {
                name += " of Vengeance";
                suffix = Suffix::VENGEANCE;
                strengthBonus = shrt(strengthBonus * 1.1f);
                return;
            }
            prb += ui16(Suffix::VENGEANCE);
            if (num > prb && num <= prb + ui16(Suffix::SLAYER)) {
                name += " of the Slayer";
                suffix = Suffix::SLAYER;
                strengthBonus = shrt(strengthBonus * 1.2f);
                return;
            }
            prb += ui16(Suffix::SLAYER);
            if (num > prb && num <= prb + ui16(Suffix::EXECUTIONER)) {
                name += " of the Executioner";
                suffix = Suffix::EXECUTIONER;
                strengthBonus = shrt(strengthBonus * 1.3f);
                return;
            }
        }

    uint16_t Weapon::levelUp() {
        uint16_t bonus = 0;
        uint16_t levels = 0;
        while (exp >= nextLevel) {
            bonus += shrt(std::round(pow(level, Data[ui16(itemType)].strMod / 2))); // level to the power of strMod / 2
            Leveled::levelUp();
            levels++;
        }
        if (levels > 0) {
            type("\nYour ", name, " leveled up");
            if (levels == 2)
                type(" twice");
            else if (levels == 3)
                type(" three times");
            else if (levels > 3)
                type(" many times");
            type("! It is now level ", level, ".\n");
        }
        strengthBonus += bonus;
        return bonus;
    }
    
    void Weapon::displayInfo(const Source source) const {
        Item::displayInfo(source);
        type(" (Strength Bonus: ", strengthBonus, ")\n");
    }
    
    // ---------- SPECIAL ---------- //
    Special::Special() : defenseBonus(0), strengthBonus(0) {}
    
    Special::Special(const TYPE splType, const uint16_t level) :
        Item(splType),
        defenseBonus (Data[ui16(splType)].defMod == 0 // if defMod is 0
            ? 0
            : shrt (
                randint ( // a random number between 1 and the level to the power of defMod
                    1,
                    ui16(std::round(pow(level, Data[ui16(splType)].defMod)))
                ) *
                (Data[ui16(splType)].nDef // ...multiplied by -1 if nDef is true
                    ? -1
                    : 1
                )
            )
        ),
        strengthBonus (Data[ui16(splType)].strMod == 0 // if strMod is 0
            ? 0
            : shrt (
                randint ( // a random number between 1 and the level to the power of strMod
                    1,
                    ui16(std::round(pow(level, Data[ui16(splType)].strMod)))
                ) *
                (Data[ui16(splType)].nStr // ...multiplied by -1 if nStr is true
                    ? -1
                    : 1
                )
            )
        ) {}
    
    void Special::displayInfo(const Source source) const {
        Item::displayInfo(source);
        type(" (Defense Bonus: ", defenseBonus, ", Strength Bonus: ", strengthBonus, ")\n");
    }

}