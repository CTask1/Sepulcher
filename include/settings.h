//CTask
#pragma once
#include<fstream>

#include "TextManager.h"
#include "Choice.h"
#include "util.h"

void changeOutputSpeed() {
    type(TM::get("settings.speed.choose_option"));
    setMode(LIST_OUT);
    type(TM::getAllAsStr("settings.speed.options", {
        .replacements = {
            {"{cur1}", TM::getForCondition("settings.current_flag", defDelay == 0)},
            {"{cur2}", TM::getForCondition("settings.current_flag", defDelay == 1)},
            {"{cur3}", TM::getForCondition("settings.current_flag", defDelay == 2)},
            {"{cur4}", TM::getForCondition("settings.current_flag", defDelay == 10)}
        }
    }));

    switch (Choice::getChoice(TM::getAllAsLst("settings.speed.options"))) {
        case 1: defDelay = 0; break;
        case 2: defDelay = 1; break;
        case 3: defDelay = 2; break;
        case 4: defDelay = 10; break;
    }
    setOutputSettings();
}

void changeOutputMode() {
    type(TM::get("settings.mode.choose_option"));
    setMode(LIST_OUT);
    type(TM::getAllAsStr("settings.mode.options", {
        .replacements = {
            {"{cur1}", TM::getForCondition("settings.current_flag", !defMode)},
            {"{cur2}", TM::getForCondition("settings.current_flag", defMode)}
        }
    }));

    switch (Choice::getChoice(TM::getAllAsLst("settings.mode.options"))) {
        case 1: defMode = 0; break;
        case 2: defMode = 1; break;
    }
    setOutputSettings();
}

void changePrompt() {
    type(TM::get("settings.prompt.choose_option"));
    setMode(LIST_OUT);
    std::vector<std::string> options = TM::getAllAsLst("settings.prompt.options");
    std::string lowPrompt = lowerFirst(trim(std::move(prompt)));
    type(TM::getAllAsStr("settings.prompt.options", {
        .replacements = {
            {"{cur1}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(0))},
            {"{cur2}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(1))},
            {"{cur3}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(2))},
            {"{cur4}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(3))},
            {"{cur5}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(4))},
            {"{cur6}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(5))}
        }
    }));

    int choice = Choice::getChoice(options);
    if (choice != 7)
        prompt = sentenceCase(choice == 6 ? trim(input(TM::get("settings.prompt.custom", {.end = ' '}).c_str())) : options.at(choice - 1)) + ' ';
}

void saveSettings() {
    std::ofstream settingsFile("settings.txt", std::ios::trunc);
    if (!settingsFile.is_open()) {
        type(TM::get("settings.save_error"));
        return;
    }
    settingsFile << defMode << '\n' << defDelay << '\n' << prompt << '\n';
    settingsFile.close();
    type(TM::get("settings.save"));
}

void resetToDefaults() {
    defMode = DEF_OUTPUT_MODE;
    defDelay = DEF_DELAY;
    prompt = "Enter choice: ";
    setOutputSettings();
    type(TM::get("settings.reset"));
}

void reloadTextFile() {
    TM::load("res\\text.json");
    type(TM::get("settings.reload"));
}

void settings() {
    while (true) {
        type(TM::get("settings.choose_option"));
        setMode(LIST_OUT);
        type(TM::getAllAsStr("settings.options"));

        switch (Choice::getChoice(TM::getAllAsLst("settings.options"))) {
            case 1: changeOutputSpeed(); break;
            case 2: changeOutputMode(); break;
            case 3: changePrompt(); break;
            case 4: saveSettings(); break;
            case 5: resetToDefaults(); break;
            case 6: reloadTextFile(); break;
            case 7: return;
        }
    }
}