#pragma once
#pragma warning(disable: 4717)
#include<type_traits>
#include<iostream>
#include<sstream>
#include<random>
#include<thread>
#include<chrono>
#include<string>
#include<cmath>

_NODISCARD inline std::string tostring() { return ""; }

template<typename T>
typename std::enable_if<!std::is_same<T, const char*>::value, std::string>::type
_NODISCARD inline tostring(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

_NODISCARD inline std::string tostring(const char* const& value) {
    return std::string(value);
}

template<typename T, typename... Args>
_NODISCARD inline std::string tostring(const T& value, const Args&... args) {
    return tostring(value) + tostring(args...);
}

_NODISCARD inline std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

_NODISCARD inline const uint16_t randint(uint16_t min, uint16_t max) {
    if (min > max) [[unlikely]]
        return 0;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

inline void wait(const uint16_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

template<typename... Args>
inline void print(const Args&... args) {
    std::cout << (... << args);
}

namespace {

    bool outputOptions = false;
    uint16_t outputDelay = 2;

}

inline void setOutputSettings(const bool options = false, const uint16_t delay = 2) {
    outputOptions = options;
    outputDelay = delay;
}

template<typename... Args>
void type(const Args&... args) {
    std::string text = tostring(args...);
    if (outputOptions) [[unlikely]] {
        while (!text.empty()) {
            print(text.substr(0, text.find('\n', 1)));
            text.erase(0, text.find('\n', 1));
            wait(outputDelay * 75);
        }
    } else {
        for (char character : text) {
            print(character);
            wait(outputDelay);
        }
    }
    setOutputSettings();
}

class Choice {
public:
    std::string choice;

    Choice() : choice("") {}

    _NODISCARD inline const bool isChoice() const { return false; }

    template<typename T, typename... Args>
    _NODISCARD inline const bool isChoice(const T& chk1, const Args&... args) const {
        return choice == tostring(chk1) || isChoice(args...);
    }

    template<typename... Args>
    _NODISCARD inline const bool isChoice(const bool showMessage = false, const Args&... args) const {
        const bool CHECK_RESULT = isChoice(args...);
        if (!CHECK_RESULT && showMessage)
            type("\nThat's not an option!\n");
        return CHECK_RESULT;
    }

    inline const Choice& operator=(const std::string& s) {
        choice = toLower(tostring(s));
        return *this;
    }

    _NODISCARD inline operator std::string() const {
        return choice;
    }

};

_NODISCARD inline const bool isInt(const std::string& s) {
    return s.find_first_not_of("-0123456789") == std::string::npos;
}

_NODISCARD inline const bool isPos(const std::string& s) {
    return s.find_first_not_of("0123456789") == std::string::npos;
}

namespace {

    template<typename... Args>
    inline std::string inputHelper(const bool isLine, const Args&... args) {
        std::string entry;
        type(args...);
        if (isLine) [[unlikely]]
            std::getline(std::cin >> std::ws, entry);
        else
            std::cin >> entry;
        return entry;
    }

}

template<typename... Args>
_NODISCARD inline std::string input(const Args&... args) {
    return inputHelper(false, args...);
}

template<typename... Args>
_NODISCARD inline std::string inputline(const Args&... args) {
    return inputHelper(true, args...);
}

inline void wheel(const uint16_t duration = 4) {
    std::string chars = "|/-\\";
    for (uint16_t rep = 0; rep < duration * 5; rep++) {
        std::cout << chars[rep % 5];
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "\r";
    }
}