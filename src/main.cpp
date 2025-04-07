//CTask1
#include<iostream>

#include"..\include\Game.h"

#ifdef _WIN32
    #include<Windows.h>
    #define SET_UTF8() SetConsoleOutputCP(CP_UTF8)
#else
    #define SET_UTF8() // Got this idea from The Cherno
#endif

int main() {
    SET_UTF8(); 
    start();
    std::cout << "\nThanks for playing!\n"
              << "\nPress Enter key to continue . . . ";
    std::cin.get();
    return 0;
}