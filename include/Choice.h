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
    _NODISCARD operator std::string() const;
    _NODISCARD bool isChoice(const char* const& ch, const int num, const bool showMessage = false) const;
    _NODISCARD int isChoice(const cstrList_t& choices, const bool showMessage = true) const;
    _NODISCARD int isChoice(const std::vector<const char*>& choices, const bool showMessage = true) const;

};