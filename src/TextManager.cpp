//CTask
#include<initializer_list>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

#include"TextManager.h"
#include"util.h"

nlohmann::json TextManager::textData;

nlohmann::json getData(const char* const& key, nlohmann::json current) {
    std::istringstream iss(key);
    std::string str;

    while (std::getline(iss, str, '.')) {
        if (!current.contains(str))
            throw std::runtime_error("Missing text key: " + STR(key));
        current = current[str];
    }

    return current;
}

void TextManager::load(const char* const& filename) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Could not open file: " + STR(filename));
    file >> textData;
    file.close();
}

void TextManager::print(const char* const& key, const OptionalGetArgs& args) {
    nlohmann::json current = getData(key, textData);
    STR str = "";

    if (current.is_array()) {
        for (size_t i = 0; i < current.size(); i++)
            str += TO_STR(i + 1) + ". " + current.at(i).get<STR>() + '\n';
    } else
        str = current.get<STR>() + args.end;

    for (const auto& [from, to] : args.replacements)
        replace(str, from, to);

    type(std::move(str));
}

std::string TextManager::get(const char* const& key, const OptionalGetArgs& args) {
    nlohmann::json current = getData(key, textData);
    std::string str = current.get<std::string>() + args.end;

    for (const auto& [from, to] : args.replacements)
        replace(str, from, to);

    return str;
}

std::string TextManager::getAllAsStr(const char* const& key, const OptionalGetArgs& args) {
    nlohmann::json current = getData(key, textData);
    std::string str = "";

    for (size_t i = 0; i < current.size(); i++)
        str += TO_STR(i + 1) + ". " + current.at(i).get<std::string>() + '\n';

    for (const auto& [from, to] : args.replacements)
        replace(str, from, to);

    return str;
}

std::vector<std::string> TextManager::getAllAsLst(const char* const& key, const OptionalGetArgs& args) {
    std::vector<std::string> items = getData(key, textData).get<std::vector<std::string>>();
    for (std::string& str : items) {
        str = lowerFirst(std::move(str));
        for (const auto& [from, to] : args.replacements)
            replace(str, from, to);
        
        str = trim(str);
    }
    
    return items;
}

std::string TextManager::getForCondition(const std::string& path, const bool condition, const OptionalGetArgs& args) {
    return get((path + '.' + (condition ? "true" : "false")).c_str(), args);
}

std::string TextManager::getForNumber(const std::string& path, const uint16_t num, const OptionalGetArgs& args) {
    return get((path + '.' + TO_STR(num)).c_str(), args);
}

std::string TextManager::trim(std::string& str) {
    size_t idx = str.find('(');
    if (idx == 0)
        idx = std::string::npos;
    else if (idx != std::string::npos)
        str.erase(idx);

    idx = str.find('{');
    if (idx != std::string::npos)
        str.erase(idx);

    return ::trim(std::move(str));
}

void TextManager::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t idx = 0;
    
    while ((idx = str.find(from, idx)) != std::string::npos) {
        str.replace(idx, from.length(), to);
        idx += to.length();
    }
}