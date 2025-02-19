#pragma once
#include"Resources.h"
#include"Util.h"

class TraderResources : public Resources {
public:
    std::unordered_map<std::string, uint16_t> prices;

    TraderResources() {
        do {
            resources["fiber"] = (randint(0, 5));
            resources["wood"] = randint(0, 5);
            resources["stone"] = randint(0, 5);
            resources["leather"] = randint(0, 5);
            resources["crystals"] = randint(0, 3);
            resources["iron"] = randint(0, 3);
        } while (resources.empty());

        for (std::pair<std::string, uint16_t> pair : resources)
            if (pair.first == "crystals" || pair.first == "iron")
                prices[pair.first] = 5;
            else
                prices[pair.first] = 1;
    }

    void displayResources() {
        type("\nTrader's resources:\n");
        for (const std::pair<std::string, uint16_t> stat : resources)
            if (stat.second != 0)
                type(true, "\t", stat.first, ": ", stat.second, " -- Price: ", prices[stat.first], "\n");
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