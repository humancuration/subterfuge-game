#pragma once
#include <string>
#include <vector>

struct EnvironmentalState {
    float temperature{20.0f};
    float humidity{0.5f};
    float pollution_level{0.0f};
    float resource_abundance{1.0f};
    float biodiversity{1.0f};
    
    std::vector<std::string> active_conditions;
    
    bool is_stressed() const {
        return pollution_level > 0.7f || 
               resource_abundance < 0.3f || 
               biodiversity < 0.3f;
    }
    
    std::vector<std::string> get_relevant_factors() const {
        std::vector<std::string> factors;
        
        if (temperature > 30.0f) factors.push_back("high_temperature");
        if (temperature < 10.0f) factors.push_back("low_temperature");
        if (humidity > 0.7f) factors.push_back("high_humidity");
        if (humidity < 0.3f) factors.push_back("low_humidity");
        if (pollution_level > 0.5f) factors.push_back("polluted");
        if (resource_abundance < 0.5f) factors.push_back("resource_scarce");
        if (biodiversity < 0.5f) factors.push_back("low_biodiversity");
        
        return factors;
    }
}; 