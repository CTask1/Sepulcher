//CTask
#pragma once
#include<nlohmann\json.hpp>
#include<initializer_list>
#include<string>
#include<vector>

#include "util.h"

class TextManager {
private:
    static nlohmann::json textData;

    struct OptionalGetArgs {
        const strPairVec_t replacements = {};
        const char end = '\n';
    };

public:

    static void load(const char* const& filename);
    static std::string get(const char* const& key, const OptionalGetArgs& args = {});
    static std::string getAllAsStr(const char* const& key, const OptionalGetArgs& args = {});
    static std::vector<std::string> getAllAsLst(const char* const& key, const OptionalGetArgs& args = {});
    static std::string getForCondition(const std::string& path, const bool condition, const OptionalGetArgs& args = {.end = '\0'});
    static std::string getForNumber(const std::string& path, const uint16_t num, const OptionalGetArgs& args = {.end = '\0'});
    static std::string trim(std::string& str);
    static void replace(std::string& str, const std::string& from, const std::string& to);

};

using TM = TextManager;