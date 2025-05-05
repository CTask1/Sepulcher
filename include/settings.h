//CTask
#pragma once
#include"pch.h"

#include "Choice.h"
#include "globals.h"
#include "util.h"

void changeOutputSpeed() {
    setList(true);
    type (
        "\nWhat would you like the output speed to be?"
        "\n1. Instant",            (defDelay == 0  ? " (current)" : ""),
        "\n2. Fast",               (defDelay == 1  ? " (current)" : ""),
        "\n3. Moderate (default)", (defDelay == 2  ? " (current)" : ""),
        "\n4. Slow",               (defDelay == 10 ? " (current)" : ""),
        "\n5. (go back)\n"
    );
    int speedChoice;
    do speedChoice = Choice(input(prompt.data())).isChoice({"instant", "fast", "moderate", "slow", "(go back)"});
    while (speedChoice == 0);

    switch (speedChoice) {
        case 1: defDelay = 0; break;
        case 2: defDelay = 1; break;
        case 3: defDelay = 2; break;
        case 4: defDelay = 10; break;
    }
    setOutputSettings();
}

void changeOutputMode() {
    setList(true);
    type (
        "\nWhat would you like the output mode to be?"
        "\n1. Text (default)", (defList ? "" : " (current)"),
        "\n2. List",           (defList ? " (current)" : ""),
        "\n3. (go back)\n"
    );
    int modeChoice;
    do modeChoice = Choice(input(prompt.data())).isChoice({"text", "list", "(go back)"});
    while (modeChoice == 0);

    switch (modeChoice) {
        case 1: defList = 0; break;
        case 2: defList = 1; break;
    }
    setOutputSettings();
}

void changePrompt() {
    setList(true);
    type (
        "\nWhat would you like the prompt to be?"
        "\n1. Enter choice: (default)", (prompt == "Enter choice: " ? " (current)" : ""),
        "\n2. What say you?",           (prompt == "What say you? " ? " (current)" : ""),
        "\n3. What is your choice?",    (prompt == "What is your choice? " ? " (current)" : ""),
        "\n4. ->",                      (prompt == "-> " ? " (current)" : ""),
        "\n5. >",                       (prompt == "> " ? " (current)" : ""),
        "\n6. (custom)",                (prompt != "Enter choice: " && prompt != "What say you? " && prompt != "What is your choice? " && prompt != "-> " && prompt != "> " ? " (current)" : ""),
        "\n7. (go back)\n"
    );
    int promptChoice;
    do promptChoice = Choice(input(prompt.data())).isChoice({"enter choice:", "what say you?", "what is your choice?", "->", ">", "(custom)", "(go back)"});
    while (promptChoice == 0);

    switch (promptChoice) {
        case 1: prompt = "Enter choice: "; break;
        case 2: prompt = "What say you? "; break;
        case 3: prompt = "What is your choice? "; break;
        case 4: prompt = "-> "; break;
        case 5: prompt = "> "; break;
        case 6: prompt = trim(input("\nEnter custom prompt: ")) + ' '; break;
    }
}

void saveSettings() {
    std::ofstream settingsFile("settings.txt");
    if (settingsFile.is_open()) {
        settingsFile << defList << '\n' << defDelay << '\n' << prompt << '\n';
        settingsFile.close();
        type("\nSettings saved.\n");
    } else {
        type("\nUnable to open settings file.\n");
    }
}

void resetToDefaults() {
    defList = DEF_OUTPUT_AS_LIST;
    defDelay = DEF_DELAY;
    prompt = "Enter choice: ";
    setOutputSettings();
    type("\nSettings reset to defaults.\n");
}

void settings() {
    while (true) {
        setList(true);
        type (
            "\nWhat would you like to do?"
            "\n1. Change output speed"
            "\n2. Change output mode"
            "\n3. Change prompt"
            "\n4. Save settings"
            "\n5. Reset to defaults"
            "\n6. Go back\n"
        );
        int choice = 0;
        do choice = Choice(input(prompt.data())).isChoice({"change output speed", "change output mode", "change prompt", "save settings", "reset to defaults", "go back"});
        while (choice == 0);

        switch (choice) {
            case 1: changeOutputSpeed(); break;
            case 2: changeOutputMode(); break;
            case 3: changePrompt(); break;
            case 4: saveSettings(); break;
            case 5: resetToDefaults(); break;
            case 6: return;
        }
    }
}