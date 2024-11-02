#pragma once
#include <string>
#include <vector>

struct Pattern {
    std::string id;
    std::vector<std::string> actions;
    std::vector<std::string> environmental_factors;
    std::vector<std::string> cultural_elements;
    
    float success_rate{0.0f};
    size_t observation_count{0};
    bool is_verified{false};
    
    // Pattern metadata
    struct Metadata {
        float complexity{0.0f};
        float stability{1.0f};
        float innovation_level{0.0f};
        std::vector<std::string> prerequisites;
    } metadata;
}; 