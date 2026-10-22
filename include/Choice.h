//CTask
#pragma once
#include<initializer_list>
#include<string>
#include<vector>

#include"util.h"

struct Choice {
    std::string choice;

    Choice();
    Choice(std::string&& str);
    Choice& operator=(std::string str);
    [[nodiscard]] operator STR() const;
    [[nodiscard]] bool isChoice(const char* const& ch, const int num, const bool showMessage = false) const;
    [[nodiscard]] uint16_t isChoice(const cstrList_t& choices, const bool showMessage = true) const;
    [[nodiscard]] uint16_t isChoice(const std::vector<const char*>& choices, const bool showMessage = true) const;
    [[nodiscard]] uint16_t isChoice(const std::vector<std::string>& choices, const bool showMessage = true) const;
    [[nodiscard]] static uint16_t getChoice(const cstrList_t& choices, const char* const& message = nullptr);
    [[nodiscard]] static uint16_t getChoice(const std::vector<const char*>& choices, const char* const& message = nullptr);
    [[nodiscard]] static uint16_t getChoice(const std::vector<std::string>& choices, const char* const& message = nullptr);

};
