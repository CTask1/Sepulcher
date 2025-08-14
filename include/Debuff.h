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

    const struct Info {
        std::string_view name = "None";
        uint16_t duration = 1;
        float hMod   = 0.f,
              strMod = 0.f,
              defMod = 0.f;
    };

    static constexpr Info Data[] {
        { "None"                                       },
        { "Ravenous" , DAY_LENGTH, -0.1f               },
        { "Exhausted", UINT16_MAX,  0.0f, -0.1f, -0.1f }
    };

    Debuff(TYPE debuffType) :
        name     (Data[debuffType].name    ),
        duration (Data[debuffType].duration),
        hMod     (Data[debuffType].hMod    ),
        strMod   (Data[debuffType].strMod  ),
        defMod   (Data[debuffType].defMod  ) {}

};
    