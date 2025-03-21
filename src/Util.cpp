#include"..\include\Util.h"

bool outputAsList = false;
uint16_t outputDelay = 2;
static std::mt19937 gen(std::random_device{}());

_NODISCARD std::string tostring() { return ""; }

_NODISCARD std::string tostring(const char* const& value) {
    return std::string(value);
}

_NODISCARD std::string toLower(std::string&& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](int ch) { return std::tolower(ch); });
    return str;
}

_NODISCARD uint16_t randint(const uint16_t min, const uint16_t max) {
    if (min > max) [[unlikely]]
        return 0;
    return std::uniform_int_distribution<uint16_t>{min, max}(gen);
}

void wait(const uint16_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

_NODISCARD bool isInt(const std::string& str) {
    return str.find_first_not_of("0123456789", (str[0] == '-' && str.length() > 1 ? 1 : 0)) == std::string::npos;
}

_NODISCARD bool isPos(const std::string& str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

void wheel(const uint16_t duration) {
    const std::string_view chars = "|/-\\";
    for (uint16_t rep = 0; rep < duration * 5; rep++) {
        std::cout << chars[rep % 4];
        wait(100);
        std::cout << "\r";
    }
}

Choice::Choice() : choice("") {}

Choice& Choice::operator=(std::string str) {
    choice = toLower(std::move(str));
    return *this;
}

_NODISCARD Choice::operator std::string() const {
    return choice;
}

_NODISCARD bool Choice::isChoice() const { return false; }