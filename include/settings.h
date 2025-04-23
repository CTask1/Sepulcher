#pragma once

#include<fstream>

#include "Choice.h"
#include "globals.h"
#include "util.h"

void settings() {
    while (true) {
        setOutputSettings(true);
        type (
            "\nWhat would you like to do?"
            "\n1. Change output speed"
            "\n2. Change output mode"
            "\n3. Change prompt"
            "\n4. Save settings"
            "\n5. Reset to defaults"
            "\n6. Go back\n"
        );
        Choice choice;
        do choice = input(prompt.data());
        while (!choice.isChoice (true, {
            { "change output speed", 1 },
            { "change output mode" , 2 },
            { "change prompt"      , 3 },
            { "save settings"      , 4 },
            { "reset to defaults"  , 5 },
            { "go back"            , 6 }
        }));

        if (choice.isChoice({{"change output speed", 1}})) {
            setOutputSettings(true);
            type (
                "\nWhat would you like the output speed to be?"
                "\n1. Instant",            (defDelay == 0  ? " (current)" : ""),
                "\n2. Fast",               (defDelay == 1  ? " (current)" : ""),
                "\n3. Moderate (default)", (defDelay == 2  ? " (current)" : ""),
                "\n4. Slow",               (defDelay == 10 ? " (current)" : ""),
                "\n5. (go back)\n"
            );
            Choice speedChoice;
            do speedChoice = input(prompt.data());
            while (!speedChoice.isChoice (true, {
                { "instant"  , 1 },
                { "fast"     , 2 },
                { "moderate" , 3 },
                { "slow"     , 4 },
                { "(go back)", 5 }
            }));

            if (speedChoice.isChoice({{"instant", 1}})) {
                defDelay = 0;
            } else if (speedChoice.isChoice({{"fast", 2}})) {
                defDelay = 1;
            } else if (speedChoice.isChoice({{"moderate", 3}})) {
                defDelay = 2;
            } else if (speedChoice.isChoice({{"slow", 4}})) {
                defDelay = 10;
            }
            setOutputSettings();
        } else if (choice.isChoice({{"change output mode", 2}})) {
            setOutputSettings(true);
            type (
                "\nWhat would you like the output mode to be?"
                "\n1. Text (default)", (defList ? "" : " (current)"),
                "\n2. List",           (defList ? " (current)" : ""),
                "\n3. (go back)\n"
            );
            Choice modeChoice;
            do modeChoice = input(prompt.data());
            while (!modeChoice.isChoice (true, {
                { "text"     , 1 },
                { "list"     , 2 },
                { "(go back)", 3 }
            }));

            if (modeChoice.isChoice({{"text", 1}})) {
                defList = false;
            } else if (modeChoice.isChoice({{"list", 2}})) {
                defList = true;
            }
            setOutputSettings();
        } else if (choice.isChoice({{"change prompt", 3}})) {
            setOutputSettings(true);
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
            Choice promptChoice;
            do promptChoice = input(prompt.data());
            while (!promptChoice.isChoice (true, {
                { "enter choice:"       , 1 },
                { "what say you?"       , 2 },
                { "what is your choice?", 3 },
                { "->"                  , 4 },
                { ">"                   , 5 },
                { "(custom)"            , 6 },
                { "(go back)"           , 7 }
            }));

            if (promptChoice.isChoice({{"enter choice:", 1}})) {
                prompt = "Enter choice: ";
            } else if (promptChoice.isChoice({{"what say you?", 2}})) {
                prompt = "What say you? ";
            } else if (promptChoice.isChoice({{"what is your choice?", 3}})) {
                prompt = "What is your choice? ";
            } else if (promptChoice.isChoice({{"->", 4}})) {
                prompt = "-> ";
            } else if (promptChoice.isChoice({{">", 5}})) {
                prompt = "> ";
            } else if (promptChoice.isChoice({{"(custom)", 6}})) {
                prompt = trim(input("\nEnter custom prompt: ")) + ' ';
            }
        } else if (choice.isChoice({{"save settings", 4}})) {
            std::ofstream settingsFile("settings.txt");
            if (settingsFile.is_open()) {
                settingsFile << outputAsList << '\n' << outputDelay << '\n' << prompt << '\n';
                settingsFile.close();
                type("\nSettings saved.\n");
            } else {
                type("\nUnable to open settings file.\n");
            }
        } else if (choice.isChoice({{"reset to defaults", 5}})) {
            outputAsList = DEF_OUTPUT_AS_LIST;
            outputDelay = DEF_DELAY;
            prompt = "Enter choice: ";
            type("\nSettings reset to defaults.\n");
        } else if (choice.isChoice({{"go back", 6}})) return;
    }
}