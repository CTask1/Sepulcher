//CTask
#pragma once

inline bool day = true;
inline bool statueFound = false;

constexpr unsigned short DAY_LENGTH = 8;
constexpr unsigned short NIGHT_LENGTH = 4;
constexpr unsigned short FULL_DAY_LENGTH = DAY_LENGTH + NIGHT_LENGTH;

constexpr std::pair<const char*, const char*> langWords[] {
    { "Thar"        , "King"           },
    { "Zensolet"    , "the Last Light" },
    { "Ruunanet"    , "of the Land"    },
    { "Tenanek"     , "of Shadows"     },
    { "nom"         , "name"           },
    { "Eian"        , "His"            },
    { "nir"         , "death"          },
    { "hatnulven"   , "was undone"     },
    { "ten"         , "shadow"         },
    { "shenkor"     , "all spirits"    },
    { "nodren a'eth", "linger"         }
};