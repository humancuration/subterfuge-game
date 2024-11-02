#pragma once
#include <godot_cpp/core/math.hpp>

struct EnvironmentalState {
    float pollution_level{0.0f};
    float resource_depletion{0.0f};
    float biodiversity{1.0f};
    float climate_stress{0.0f};
    
    bool is_stressed() const {
        return pollution_level > 0.7f ||
               resource_depletion > 0.7f ||
               biodiversity < 0.3f ||
               climate_stress > 0.7f;
    }
}; 