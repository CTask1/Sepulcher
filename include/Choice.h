#pragma once

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

    _NODISCARD bool isChoice() const { return false; }

    template<typename T, typename... Args>
    _NODISCARD bool isChoice(const T& chk1, const Args&... args) const {
        return choice == tostring(chk1) || isChoice(args...);
    }

    template<typename... Args>
    _NODISCARD bool isChoice(const bool showMessage = false, const Args&... args) const {
        const bool CHECK_RESULT = isChoice(args...);
        if (!CHECK_RESULT && showMessage)
            type("\nThat's not an option!\n");
        return CHECK_RESULT;
    }

};