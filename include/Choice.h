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
    _NODISCARD operator STR() const;
    _NODISCARD bool isChoice(const char* const& ch, const int num, const bool showMessage = false) const;
    _NODISCARD uint16_t isChoice(const cstrList_t& choices, const bool showMessage = true) const;
    _NODISCARD uint16_t isChoice(const std::vector<const char*>& choices, const bool showMessage = true) const;
    _NODISCARD uint16_t isChoice(const std::vector<std::string>& choices, const bool showMessage = true) const;
    _NODISCARD static uint16_t getChoice(const cstrList_t& choices, const char* const& message = nullptr);
    _NODISCARD static uint16_t getChoice(const std::vector<const char*>& choices, const char* const& message = nullptr);
    _NODISCARD static uint16_t getChoice(const std::vector<std::string>& choices, const char* const& message = nullptr);

};