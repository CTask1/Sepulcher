//CTask1
#pragma once
#include<unordered_map>
#include<string>

#include"Resources.h"
#include"Util.h"

class TraderResources : public Resources {
public:
    std::unordered_map<std::string, uint16_t> prices;

    TraderResources() {
        do {
            resources["Fiber"] = (randint(0, 5));
            resources["Wood"] = randint(0, 5);
            resources["Stone"] = randint(0, 5);
            resources["Leather"] = randint(0, 5);
            resources["Crystals"] = randint(0, 3);
            resources["Iron"] = randint(0, 3);
        } while (resources.empty());

        for (std::pair<std::string, uint16_t> pair : resources)
            if (pair.first == "Crystals" || pair.first == "Iron")
                prices[pair.first] = 5;
            else
                prices[pair.first] = 1;
    }

    void displayResources() {
        type("\nTrader's resources:\n");
        for (const std::pair<std::string, uint16_t> stat : resources)
            if (stat.second != 0) {
                setOutputSettings(true);
                type("\t", stat.first, ": ", stat.second, " -- Price: ", prices[stat.first], "\n");
            }
    }

};

class Trader {
public:
    TraderResources resources;

    Trader() {}

    void displayWares() {
        resources.displayResources();
    }
    
};