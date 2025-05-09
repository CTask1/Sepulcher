//CTask
#include<initializer_list>
#include<string>
#include<vector>

#include"Choice.h"
#include"util.h"

Choice::Choice() : choice("") {}

Choice::Choice(std::string&& str) : choice(toLower(std::move(str))) {}

Choice& Choice::operator=(std::string str) {
    choice = toLower(std::move(str));
    return *this;
}

_NODISCARD Choice::operator std::string() const {
    return choice;
}
    
_NODISCARD bool Choice::isChoice(const char* const& ch, const int num, const bool showMessage) const {
    if (this->choice == ch || this->choice == std::to_string(num))
        return true;
    if (showMessage)
        type("\nThat's not an option!\n");
    return false;
}
    
_NODISCARD int Choice::isChoice(const cstrList_t& choices, const bool showMessage) const {
    for (uint16_t i = 0; i < choices.size(); i++)
        if (isChoice(*(choices.begin() + i), i + 1, false))
            return i + 1;
    if (showMessage)
        type("\nThat's not an option!\n");
    return 0;
}

_NODISCARD int Choice::isChoice(const std::vector<const char*>& choices, const bool showMessage) const {
    for (uint16_t i = 0; i < choices.size(); i++)
        if (isChoice(choices[i], i + 1, false))
            return i + 1;
    if (showMessage)
        type("\nThat's not an option!\n");
    return 0;
}