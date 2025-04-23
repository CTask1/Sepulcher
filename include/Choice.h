#pragma once

#include<initializer_list>
#include<string>

#include"util.h"

class Choice {
public:
    std::string choice;

    Choice() : choice("") {}

    Choice& operator=(std::string str) {
        choice = toLower(std::move(str));
        return *this;
    }

    _NODISCARD operator std::string() const {
        return choice;
    }

    _NODISCARD bool isChoice(const IntPairList_t& choices) {
        for (const std::pair<std::string, int>& ch : choices) {
            if (choice == ch.first || choice == std::to_string(ch.second))
                return true;
        }
        return false;
    }

    _NODISCARD bool isChoice(const bool showMessage, const IntPairList_t& choices) {
        const bool CHECK_RESULT = isChoice(choices);
        if (!CHECK_RESULT && showMessage)
            type("\nThat's not an option!\n");
        return CHECK_RESULT;
    }

};