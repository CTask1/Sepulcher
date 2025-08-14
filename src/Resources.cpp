//CTask
#pragma once
#include<unordered_map>
#include<string>

#include"Resources.h"
#include"util.h"

Resources::Resources() = default;

Resources* Resources::addResource(const std::string& resource, const uint16_t amount) {
    if (amount > 0)
        resources[resource] += amount;
    return this;
}

void Resources::addResources(const Resources& addedResources) {
    for (const std::pair<std::string, uint16_t>& pair : addedResources.resources)
        if (pair.second > 0)
            resources[pair.first] += pair.second;
}

bool Resources::empty() const {
    return resources.empty();
}

void Resources::displayResources(const bool showTitle) const {
    if (showTitle)
        type("\nInventory:\n");
    if (resources.empty()) {
        setMode(LIST_OUT);
        type("\tNone\n");
    }
    else {
        for (const std::pair<std::string, uint16_t>& stat : resources)
            if (stat.second != 0) {
                setMode(LIST_OUT);
                type("\t" + stat.first + ": " + std::to_string(stat.second) + "\n");
            }
    }
}

uint16_t& Resources::operator[](const std::string& resource) {
    return resources[resource];
}