//CTask
#pragma once
#include<iostream>
#include<string>
#include<chrono>
#include<thread>
#include<random>

#define shrt(x) static_cast<short>(x)
#define ui16(x) static_cast<uint16_t>(x)

typedef std::initializer_list<const char* const> cstrList_t;

inline std::string prompt = "Enter choice: ";
constexpr bool DEF_OUTPUT_AS_LIST = false;
inline bool defList = DEF_OUTPUT_AS_LIST;
inline bool outputAsList = defList;
constexpr uint16_t DEF_DELAY = 2;
inline uint16_t defDelay = DEF_DELAY;
inline uint16_t outputDelay = defDelay;
inline std::mt19937 gen(std::random_device{}());

_NODISCARD inline std::string tostring() { return ""; }

_NODISCARD inline std::string tostring(const short value) {
    return std::to_string(value);
}

_NODISCARD inline std::string tostring(const uint16_t value) {
    return std::to_string(value);
}

_NODISCARD inline std::string tostring(const int value) {
    return std::to_string(value);
}

_NODISCARD inline std::string tostring(const float value) {
    return std::to_string(value);
}

_NODISCARD inline std::string tostring(const bool value) {
    return std::to_string(value);
}

_NODISCARD inline std::string tostring(const char* const& value) {
    return std::string(value);
}

_NODISCARD inline std::string tostring(const std::string_view& value) {
    return std::string(value);
}

_NODISCARD inline std::string tostring(const std::string& value) {
    return value;
}

template<typename T, typename... Args> _NODISCARD inline std::string tostring(const T& value, const Args&... args) {
    return tostring(value) + tostring(args...);
}

_NODISCARD inline std::string trim(std::string&& str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](uint8_t ch) { return !std::isspace(ch); }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](uint8_t ch) { return !std::isspace(ch); }).base(), str.end());
    return str;
}

_NODISCARD inline std::string toLower(std::string&& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](uint8_t ch) { return static_cast<uint8_t>(std::tolower(ch)); });
    return str;
}

_NODISCARD inline std::string capitalize(std::string&& str) {
    str = toLower(std::move(str));
    str[0] = (char)std::toupper(str[0]);
    for (size_t i = 2; i < str.length(); i++) { // assuming no leading spaces
        if (str[i - 1] == ' ')
            str[i] = (char)std::toupper(str[i]);
    }
    return str;
}

_NODISCARD inline bool streq(const char* const& str1, const char* const& str2) {
    return std::strcmp(str1, str2) == 0;
}

_NODISCARD inline uint16_t randint(uint16_t min, uint16_t max) {
    // swap variables
    if (min > max) [[unlikely]] {
        min ^= max;
        max ^= min;
        min ^= max;
    }
    return std::uniform_int_distribution<uint16_t>{min, max}(gen);
}

inline void wait(const uint16_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

inline void setOutputSettings(const bool list = defList, const uint16_t delay = defDelay) {
    outputAsList = list;
    outputDelay = delay;
}

template<typename... Args> inline void type(const Args&... args) {
    std::string text = tostring(args...);
    if (outputDelay == 0) {
        std::cout << text;
        return;
    }
    if (outputAsList) {
        while (!text.empty()) {
            if (text.find('\n') == std::string::npos) {
                std::cout << text << std::flush;
                wait(ui16(120 + 43.44 * log(outputDelay)));
                break;
            }
            std::cout << text.substr(0, text.find('\n') + 1) << std::flush;
            text.erase(0, text.find('\n') + 1);
            wait(ui16(120 + 43.44 * log(outputDelay)));
        }
    } else {
        for (const char character : text) {
            std::cout << character << std::flush;
            wait(outputDelay);
        }
    }
    setOutputSettings();
}

_NODISCARD inline std::string input(const char* const& prmpt) {
    std::string entry;
    type(prmpt);
    std::getline(std::cin >> std::ws, entry);
    return entry;
}

_NODISCARD inline bool isInt(const std::string& str) {
    return str.find_first_not_of("0123456789", (str[0] == '-' && str.length() > 1 ? 1 : 0)) == std::string::npos;
}

_NODISCARD inline bool isPos(const std::string& str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

inline void wheel(const uint16_t duration = 4) {
    const std::string_view chars = "|/-\\";
    for (uint16_t rep = 0; rep < duration * 5; rep++) {
        std::cout << chars[rep % 4];
        wait(100);
        std::cout << "\r";
    }
}