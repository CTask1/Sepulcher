//CTask1
#pragma once
#include<type_traits>
#include<iostream>
#include<sstream>
#include<string>
#include<chrono>
#include<thread>
#include<random>

#include"Util.h"

extern bool outputAsList;
extern uint16_t outputDelay;

template<typename T>
typename std::enable_if<!std::is_same<T, const char*>::value, std::string>::type
_NODISCARD tostring(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template<typename T, typename... Args>
_NODISCARD std::string tostring(const T& value, const Args&... args) {
    return tostring(value) + tostring(args...);
}

inline void setOutputSettings(const bool list, const uint16_t delay) {
    outputAsList = list;
    outputDelay = delay;
}

template<typename... Args>
void type(const Args&... args) {
    std::string text = tostring(args...);
    if (outputAsList) {
        while (!text.empty()) {
            if (text.find('\n') == std::string::npos) {
                std::cout << text << std::flush;
                wait(outputDelay * 75);
                break;
            }
            std::cout << text.substr(0, text.find('\n') + 1) << std::flush;
            text.erase(0, text.find('\n') + 1);
            wait(outputDelay * 75);
        }
    } else {
        for (const char character : text) {
            std::cout << character << std::flush;
            wait(outputDelay);
        }
    }
    setOutputSettings();
}

template<typename... Args>
_NODISCARD std::string input(const Args&... args) {
    std::string entry;
    type(args...);
    std::getline(std::cin >> std::ws, entry);
    return entry;
}

template<typename T, typename... Args>
_NODISCARD bool Choice::isChoice(const T& chk1, const Args&... args) const {
    return choice == tostring(chk1) || isChoice(args...);
}

template<typename... Args>
_NODISCARD bool Choice::isChoice(const bool showMessage, const Args&... args) const {
    const bool CHECK_RESULT = isChoice(args...);
    if (!CHECK_RESULT && showMessage)
        type("\nThat's not an option!\n");
    return CHECK_RESULT;
}