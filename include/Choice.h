//CTask
#pragma once
#include"pch.h"

#include"util.h"

struct Choice {
    std::string choice;

    Choice() : choice("") {}

    Choice(std::string&& str) : choice(toLower(std::move(str))) {}

    Choice& operator=(std::string str) {
        choice = toLower(std::move(str));
        return *this;
    }

    _NODISCARD operator std::string() const {
        return choice;
    }
    
    _NODISCARD bool isChoice(const char* const& ch, const int num, const bool showMessage = false) const {
        if (this->choice == ch || this->choice == std::to_string(num))
            return true;
        if (showMessage)
            type("\nThat's not an option!\n");
        return false;
    }
    
    _NODISCARD int isChoice(const cstrList_t& choices, const bool showMessage = true) const {
        return isChoiceTmp(choices, showMessage);
    }

    _NODISCARD int isChoice(const std::vector<const char*>& choices, const bool showMessage = true) const {
        return isChoiceTmp(choices, showMessage);
    }

private:

    template<typename T> _NODISCARD int isChoiceTmp(const T& choices, const bool showMessage) const {
        for (uint16_t i = 0; i < choices.size(); i++)
            if (isChoice(*(choices.begin() + i), i + 1, false))
                return i + 1;
        if (showMessage)
            type("\nThat's not an option!\n");
        return 0;
    }

};