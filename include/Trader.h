//CTask
#pragma once
#include<unordered_map>
#include<string>

#include"Resources.h"
#include"util.h"

class TraderResources : public Resources {
public:
    std::unordered_map<std::string, uint16_t> prices;

    TraderResources() {
        do {
            addResource("Medicinal Herbs", randint(0, 5))
            ->addResource("Fiber", randint(0, 5))
            ->addResource("Wood", randint(0, 5))
            ->addResource("Stone", randint(0, 5))
            ->addResource("Leather", randint(0, 5))
            ->addResource("Crystals", randint(0, 3))
            ->addResource("Iron", randint(0, 3));
        } while (resources.empty());

        for (std::pair<std::string, uint16_t> pair : resources)
            if (pair.first == "Crystals" || pair.first == "Iron")
                prices[pair.first] = 5;
            else if (pair.first == "Medicinal Herbs")
                prices[pair.first] = 2;
            else
                prices[pair.first] = 1;
    }

    void displayResources() const {
        type("\nTrader's resources:\n");
        for (const std::pair<std::string, uint16_t>& stat : resources)
            if (stat.second != 0) {
                setOutputSettings(true);
                type("\t", stat.first, ": ", stat.second, " -- Price: ", prices.at(stat.first), "\n");
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