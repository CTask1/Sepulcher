#pragma once
#include<unordered_map>
#include<string_view>

#include"Util.h"

class Resources {
public:
    std::unordered_map<std::string_view, uint16_t> resources;

    Resources() {}

    Resources* addResource(const std::string_view& resource, const uint16_t amount = 1) {
        if (amount > 0)
            resources[resource] += amount;
        return this;
    }

    void addResources(const Resources& addedResources) {
        for (const std::pair<std::string_view, uint16_t>& pair : addedResources.resources)
            if (pair.second > 0)
                resources[pair.first] += pair.second;
    }

    bool empty() const {
        return resources.empty();
    }

    void displayResources(const bool showTitle = true) const {
        if (showTitle)
            type("\nResources:\n");
        if (resources.empty()) {
            setOutputSettings(true);
            type("\tNone\n");
        }
        else {
            for (const std::pair<std::string_view, uint16_t>& stat : resources)
                if (stat.second != 0) {
                    setOutputSettings(true);
                    type("\t", stat.first, ": ", stat.second, "\n");
                }
        }
    }

    uint16_t& operator[](const std::string_view& resource) {
        return resources[resource];
    }
    
};