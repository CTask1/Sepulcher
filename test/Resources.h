#pragma once
#include<unordered_map>
#include"Util.h"
//import Util;

class Resources {
public:
    std::unordered_map<std::string, uint16_t> resources;

    Resources() {}

    Resources* addResource(const std::string& resource, const uint16_t amount = 1) {
        if (amount > 0)
            resources[resource] += amount;
        return this;
    }

    void addResources(const Resources& addedResources) {
        for (const std::pair<std::string, uint16_t>& pair : addedResources.resources)
            if (pair.second > 0)
                resources[pair.first] += pair.second;
    }

    const bool empty() const {
        return resources.empty();
    }

    void displayResources(const bool showTitle = true) const {
        if (showTitle)
            type("\nResources:\n");
        if (resources.empty())
            type(true, "\tNone\n");
        else {
            for (const std::pair<std::string, uint16_t>& stat : resources)
                if (stat.second != 0)
                    type(true, "\t", stat.first, ": ", stat.second, "\n");
        }
    }

    uint16_t& operator[](const std::string& resource) {
        return resources[resource];
    }
    
};