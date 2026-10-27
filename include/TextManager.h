//CTask
#pragma once
#include<initializer_list>
#include<string>
#include<vector>

#include "nlohmann/json.hpp"
#include "util.h"

class TextManager {
private:
    static nlohmann::json textData;

    struct OptionalGetArgs {
        strPairVec_t replacements = {};
        const char* const end = "\n";
    };

public:

    static void load(const char* const& filename);
    static void print(const char* const& key, const OptionalGetArgs& args = {{}, "\n"});
    static std::string get(const char* const& key, const OptionalGetArgs& args = {{}, "\n"});
    static std::string getAllAsStr(const char* const& key, const OptionalGetArgs& args = {{}, "\n"});
    static std::vector<std::string> getAllAsLst(const char* const& key, const OptionalGetArgs& args = {{}, "\n"});
    static std::string getForCondition(const std::string& path, const bool condition, const OptionalGetArgs& args = {{}, "\n"});
    static std::string getForNumber(const std::string& path, const uint16_t num, const OptionalGetArgs& args = {{}, "\n"});
    static std::string trim(std::string& str);
    static void replace(std::string& str, const std::string& from, const std::string& to);

};

using TM = TextManager;
