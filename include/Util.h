//CTask
#pragma once
#include<iostream>
#include<sstream>
#include<chrono>
#include<random>
#include<string>
#include<thread>
#include<vector>

#define DEV_MODE 0

#define shrt(x) static_cast<short>(x)
#define ui16(x) static_cast<uint16_t>(x)
#define LIST_OUT true
#define TYPE_OUT false
#define DELAY(x) x

namespace std {
    template<typename T> class initializer_list;
}

typedef std::initializer_list<const char* const> cstrList_t;
typedef std::vector<std::pair<const std::string, const std::string>> strPairVec_t;

inline std::string prompt = "Enter choice: ";
constexpr bool DEF_OUTPUT_MODE = TYPE_OUT;
inline bool defMode = DEF_OUTPUT_MODE;
inline bool outputMode = defMode;
constexpr uint16_t DEF_DELAY = DELAY(2);
inline uint16_t defDelay = DEF_DELAY;
inline uint16_t outputDelay = defDelay;
inline std::mt19937 gen(std::random_device{}());

// split a string into a vector of strings using a specified delimiter
_NODISCARD inline std::vector<std::string> split(const char* const& str, const char delim) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, delim)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// move a word to the left of the previous word
inline void moveWordBack(std::string& str, const char* const& word) {
    // get the starting position of the word
    size_t pos = str.find(word);
    if (pos == std::string::npos || pos == 0) return;

    // get the end position of the previous word
    size_t endPrev = pos - 1;
    for (; endPrev > 0 && str[endPrev] == ' '; endPrev--);

    // get the starting position of the previous word
    size_t prevPos = str.rfind(' ', endPrev);
    if (prevPos == std::string::npos)
        prevPos = 0;
    else
        prevPos++;

    // perform the swap
    str.erase(endPrev + 1, pos - endPrev + strlen(word) - 1);
    str.insert(prevPos, std::string(word) + ' ');
}

// remove leading and trailing whitespace from a string
_NODISCARD inline std::string trim(std::string&& str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](uint8_t ch) { return !std::isspace(ch); }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](uint8_t ch) { return !std::isspace(ch); }).base(), str.end());
    return str;
}

// convert a string to lowercase
_NODISCARD inline std::string toLower(std::string&& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](uint8_t ch) { return static_cast<uint8_t>(std::tolower(ch)); });
    return str;
}

// capitalize the first letter of each word in a string
_NODISCARD inline std::string capitalize(std::string&& str) {
    str = toLower(std::move(str));
    str[0] = (char)std::toupper(str[0]);
    for (size_t i = 2; i < str.length(); i++) { // assuming no leading spaces
        if (str[i - 1] == ' ')
            str[i] = (char)std::toupper(str[i]);
    }
    return str;
}

_NODISCARD inline std::string sentenceCase(std::string&& str) {
    str = toLower(std::move(str));
    str[0] = (char)std::toupper(str[0]);
    return str;
}

_NODISCARD inline std::string lowerFirst(std::string&& str) {
    str[0] = static_cast<char>(std::tolower(str[0]));
    return str;
}

// check if two strings are equal
_NODISCARD inline bool streq(const char* const& str1, const char* const& str2) {
    return std::strcmp(str1, str2) == 0;
}

// get a random number between min and max (inclusive)
_NODISCARD inline uint16_t randint(uint16_t min, uint16_t max) {
    // swap variables
    if (min > max) [[unlikely]] {
        min ^= max;
        max ^= min;
        min ^= max;
    }
    return std::uniform_int_distribution<uint16_t>{min, max}(gen);
}

// wait for a specified number of milliseconds
inline void wait(const uint16_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// set output settings for the type function
inline void setOutputSettings(const bool mode = defMode, const uint16_t delay = defDelay) {
    outputMode = mode;
    outputDelay = delay;
}

// set output mode
inline void setMode(const bool mode) {
    outputMode = mode;
}

// set output delay
inline void setDelay(const uint16_t delay) {
    outputDelay = delay;
}

// type a string to the console
inline void type(std::string&& text) {
    if (outputDelay == 0) {
        std::cout << text;
        return;
    }
    if (outputMode == LIST_OUT) {
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

// get user input from a prompt
_NODISCARD inline std::string input(const char* const& prmpt) {
    std::string entry;
    type(std::string(prmpt));
    std::getline(std::cin >> std::ws, entry);
    return entry;
}

// check if a string is an integer
_NODISCARD inline bool isInt(const std::string& str) {
    return str.find_first_not_of("0123456789", (str[0] == '-' && str.length() > 1 ? 1 : 0)) == std::string::npos;
}

// check if a string is a positive integer
_NODISCARD inline bool isPos(const std::string& str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

// display a loading wheel
inline void wheel(const uint16_t duration = 4) {
    const std::string_view chars = "|/-\\";
    for (uint16_t rep = 0; rep < duration * 5; rep++) {
        std::cout << chars[rep % 4];
        wait(100);
        std::cout << "\r";
    }
}