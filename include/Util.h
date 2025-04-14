//CTask
#pragma once
#include<type_traits>
#include<string>

_NODISCARD std::string tostring();
template<typename T> typename std::enable_if<!std::is_same<T, const char*>::value, std::string>::type _NODISCARD tostring(const T&);
_NODISCARD std::string tostring(const char* const&);
template<typename T, typename... Args> _NODISCARD std::string tostring(const T&, const Args&...);
_NODISCARD std::string toLower(std::string&&);
_NODISCARD uint16_t randint(uint16_t, uint16_t);
void wait(const uint16_t);
void setOutputSettings(const bool = false, const uint16_t = 2);
template<typename... Args> void type(const Args&...);
_NODISCARD std::string capitalize(std::string&&);
_NODISCARD bool isInt(const std::string&);
_NODISCARD bool isPos(const std::string&);
template<typename... Args> _NODISCARD std::string input(const Args&...);
void wheel(const uint16_t = 4);

class Choice {
public:
    std::string choice;
    
    Choice();
    Choice& operator=(std::string);
    _NODISCARD operator std::string() const;
    _NODISCARD bool isChoice() const;
    template<typename T, typename... Args> _NODISCARD bool isChoice(const T& chk1, const Args&...) const;
    template<typename... Args> _NODISCARD bool isChoice(const bool = false, const Args&...) const;

};

#include"Util.tpp"