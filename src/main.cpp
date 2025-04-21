//CTask
#include<iostream>
#include<fstream>
#include<string>

#include"..\include\game.h"
#include"..\include\util.h"

#ifdef _WIN32
    #include<Windows.h>
    #define SET_UTF8() SetConsoleOutputCP(CP_UTF8)
#else
    #define SET_UTF8() // Got this idea from The Cherno
#endif

void loadSettings() {
    std::ifstream file("settings.txt");
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        defList = line == "1";
        std::getline(file, line);
        defDelay = ui16(std::stoi(line));
        std::getline(file >> std::ws, line);
        prompt = line;
        file.close();
    }
}

int main() {
    SET_UTF8();
    loadSettings();
    start();
    std::cout << "\nThanks for playing!\n"
              << "\nPress Enter key to continue . . . ";
    std::cin.get();
    return 0;
}