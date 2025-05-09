//CTask
#pragma once
#include<unordered_map>
#include<string>

class Resources {
public:
    std::unordered_map<std::string, uint16_t> resources;

    Resources();

    Resources* addResource(const std::string& resource, const uint16_t amount = 1);
    void addResources(const Resources& addedResources);
    bool empty() const;
    void displayResources(const bool showTitle = true) const;
    uint16_t& operator[](const std::string& resource);
    
};