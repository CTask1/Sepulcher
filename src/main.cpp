//CTask
#include<iostream>
#include<fstream>
#include<string>

#include"TextManager.h"
#include"game.h"
#include"util.h"

#ifdef _WIN32
    extern "C" __declspec(dllimport) int __stdcall SetConsoleOutputCP(uint32_t);
    #define SET_UTF8() SetConsoleOutputCP(65001)
#else
    #define SET_UTF8() // Got this idea from The Cherno
#endif

void loadSettings() {
    std::ifstream file("settings.txt");
    if (!file.is_open())
        return;
    std::string line;
    std::getline(file, line);
    defMode = line == "1";
    std::getline(file, line);
    defDelay = ui16(std::stoi(line));
    std::getline(file >> std::ws, line);
    prompt = line;
    file.close();
}

int main() {
    SET_UTF8();
    loadSettings();
    TM::load("res\\text.json");
    start();
    std::cout << "\nThanks for playing!\n"
              << "\nPress Enter key to continue . . . ";
    std::cin.get();
    return 0;
}