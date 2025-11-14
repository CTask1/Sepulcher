//CTask
#include<iostream>

#include"TextManager.h"
#include"game.h"
#include"util.h"

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