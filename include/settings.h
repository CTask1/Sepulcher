//CTask
#pragma once
#include<fstream>

#include "TextManager.h"
#include "Choice.h"
#include "util.h"

void changeOutputSpeed() {
    TM::print("settings.speed.choose_option");
    setMode(LIST_OUT);
    TM::print("settings.speed.options", {
        .replacements = {
            {"{cur1}", TM::getForCondition("settings.current_flag", defDelay == 0, {.end = ""})},
            {"{cur2}", TM::getForCondition("settings.current_flag", defDelay == 1, {.end = ""})},
            {"{cur3}", TM::getForCondition("settings.current_flag", defDelay == 2, {.end = ""})},
            {"{cur4}", TM::getForCondition("settings.current_flag", defDelay == 10, {.end = ""})}
        }, .end = ""
    });

    switch (Choice::getChoice(TM::getAllAsLst("settings.speed.options"))) {
        case 1: defDelay = 0; break;
        case 2: defDelay = 1; break;
        case 3: defDelay = 2; break;
        case 4: defDelay = 10; break;
    }
    setOutputSettings();
}

void changeOutputMode() {
    TM::print("settings.mode.choose_option");
    setMode(LIST_OUT);
    TM::print("settings.mode.options", {
        .replacements = {
            {"{cur1}", TM::getForCondition("settings.current_flag", !defMode, {.end = ""})},
            {"{cur2}", TM::getForCondition("settings.current_flag", defMode, {.end = ""})}
        }, .end = ""
    });

    switch (Choice::getChoice(TM::getAllAsLst("settings.mode.options"))) {
        case 1: defMode = 0; break;
        case 2: defMode = 1; break;
    }
    setOutputSettings();
}

void changePrompt() {
    TM::print("settings.prompt.choose_option");
    setMode(LIST_OUT);
    std::vector<std::string> options = TM::getAllAsLst("settings.prompt.options");
    std::string lowPrompt = lowerFirst(trim(std::move(prompt)));
    TM::print("settings.prompt.options", {
        .replacements = {
            {"{cur1}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(0), {.end = ""})},
            {"{cur2}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(1), {.end = ""})},
            {"{cur3}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(2), {.end = ""})},
            {"{cur4}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(3), {.end = ""})},
            {"{cur5}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(4), {.end = ""})},
            {"{cur6}", TM::getForCondition("settings.current_flag", lowPrompt == options.at(5), {.end = ""})}
        }
    });

    int choice = Choice::getChoice(options);
    if (choice != 7)
        prompt = sentenceCase(choice == 6 ? trim(input(TM::get("settings.prompt.custom", {.end = " "}).c_str())) : options.at(choice - 1)) + ' ';
}

void saveSettings() {
    std::ofstream settingsFile(getExecDir() / "settings.txt", std::ios::trunc);
    if (!settingsFile.is_open()) {
        TM::print("settings.save_error");
        return;
    }
    settingsFile << defMode << '\n' << defDelay << '\n' << prompt << '\n';
    settingsFile.close();
    TM::print("settings.save");
}

void resetToDefaults() {
    defMode = DEF_OUTPUT_MODE;
    defDelay = DEF_DELAY;
    prompt = "Enter choice: ";
    setOutputSettings();
    type(TM::get("settings.reset"));
}

void reloadTextFile() {
#ifdef NDEBUG
    TM::print("settings.no_reload");
    return;
#endif
    TM::load("res/text.json");
    TM::print("settings.reload");
}

void settings() {
    while (true) {
        TM::print("settings.choose_option");
        setMode(LIST_OUT);
        TM::print("settings.options");

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
