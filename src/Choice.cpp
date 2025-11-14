//CTask
#include<initializer_list>
#include<string>
#include<vector>

#include"Choice.h"
#include"util.h"

_NODISCARD inline bool checkMessage(const bool showMessage) {
    if (showMessage)
        type("\nThat's not an option!\n");
    return false;
}

Choice::Choice() : choice("") {}

Choice::Choice(std::string&& str) : choice(toLower(std::move(str))) {}

Choice& Choice::operator=(std::string str) {
    choice = toLower(std::move(str));
    return *this;
}

_NODISCARD Choice::operator STR() const {
    return choice;
}
    
_NODISCARD bool Choice::isChoice(const char* const& ch, const int num, const bool showMessage) const {
    if (this->choice == ch || this->choice == TO_STR(num))
        return true;
    return checkMessage(showMessage);
}
    
_NODISCARD uint16_t Choice::isChoice(const cstrList_t& choices, const bool showMessage) const {
    for (uint16_t i = 0; i < choices.size(); i++)
        if (isChoice(*(choices.begin() + i), i + 1, false))
            return i + 1;
    return checkMessage(showMessage);
}

_NODISCARD uint16_t Choice::isChoice(const std::vector<const char*>& choices, const bool showMessage) const {
    for (uint16_t i = 0; i < choices.size(); i++)
        if (isChoice(choices[i], i + 1, false))
            return i + 1;
    return checkMessage(showMessage);
}

_NODISCARD uint16_t Choice::isChoice(const std::vector<std::string>& choices, const bool showMessage) const {
    for (uint16_t i = 0; i < choices.size(); i++)
        if (isChoice(choices[i].c_str(), i + 1, false))
            return i + 1;
    return checkMessage(showMessage);
}

template<typename T>
_NODISCARD uint16_t getChoiceImpl(const T& choices, const char* const& message) {
    uint16_t choice = 0;
    do choice = Choice(input((message != nullptr ? message : prompt.data()))).isChoice(choices);
    while (choice == 0);
    
    return choice;
}

template uint16_t getChoiceImpl<cstrList_t>              (const cstrList_t&              , const char* const& message);
template uint16_t getChoiceImpl<std::vector<const char*>>(const std::vector<const char*>&, const char* const& message);
template uint16_t getChoiceImpl<std::vector<std::string>>(const std::vector<std::string>&, const char* const& message);

_NODISCARD uint16_t Choice::getChoice(const cstrList_t& choices, const char* const& message) {
    return getChoiceImpl<cstrList_t>(choices, message);
}

_NODISCARD uint16_t Choice::getChoice(const std::vector<const char*>& choices, const char* const& message) {
    return getChoiceImpl<std::vector<const char*>>(choices, message);
}

_NODISCARD uint16_t Choice::getChoice(const std::vector<std::string>& choices, const char* const& message) {
    return getChoiceImpl<std::vector<std::string>>(choices, message);
}