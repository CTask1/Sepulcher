//CTask

#include"..\include\Game.h"

#ifdef _WIN32

    #include<Windows.h>

#endif

void setupConsole() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
}

int main() {
    setupConsole();
    start();
    std::cout << "\nThanks for playing!\n\n";
    return 0;
}