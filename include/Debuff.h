//CTask
#pragma once
#include<string_view>

#include"globals.h"

class Debuff {
public:
    std::string_view name;
    uint16_t duration;
    float hMod, strMod, defMod;

    enum TYPE {
        NONE,
        RAVENOUS,
        EXHAUSTED
    };

    struct Info {
        std::string_view name;
        uint16_t duration;
        float hMod  ,
              strMod,
              defMod;
    };

    static constexpr Info Data[] {
        { "None"     ,          1,   0.f,   0.f,   0.f },
        { "Ravenous" , DAY_LENGTH, -0.1f,   0.f,   0.f },
        { "Exhausted", UINT16_MAX,  0.0f, -0.1f, -0.1f }
    };

    Debuff(TYPE debuffType) :
        name     (Data[debuffType].name    ),
        duration (Data[debuffType].duration),
        hMod     (Data[debuffType].hMod    ),
        strMod   (Data[debuffType].strMod  ),
        defMod   (Data[debuffType].defMod  ) {}

};
