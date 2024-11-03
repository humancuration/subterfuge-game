#include "ClimateSystem.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ClimateSystem::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update_climate", "delta"), &ClimateSystem::update_climate);
    ClassDB::bind_method(D_METHOD("simulate_resource_extraction", "delta"), &ClimateSystem::simulate_resource_extraction);
    
    ClassDB::bind_method(D_METHOD("get_temperature", "position"), &ClimateSystem::get_temperature);
    ClassDB::bind_method(D_METHOD("get_humidity", "position"), &ClimateSystem::get_humidity);
    ClassDB::bind_method(D_METHOD("get_air_quality", "position"), &ClimateSystem::get_air_quality);
    ClassDB::bind_method(D_METHOD("get_radiation_level", "position"), &ClimateSystem::get_radiation_level);
    
    ClassDB::bind_method(D_METHOD("add_hazard", "hazard"), &ClimateSystem::add_hazard);
    ClassDB::bind_method(D_METHOD("remove_hazard", "position", "radius"), &ClimateSystem::remove_hazard);
    ClassDB::bind_method(D_METHOD("get_active_hazards"), &ClimateSystem::get_active_hazards);
    
    ClassDB::bind_method(D_METHOD("add_extraction_site", "position", "rate"), &ClimateSystem::add_extraction_site);
    ClassDB::bind_method(D_METHOD("remove_extraction_site", "position"), &ClimateSystem::remove_extraction_site);
    ClassDB::bind_method(D_METHOD("get_resource_concentration", "position"), &ClimateSystem::get_resource_concentration);
}

ClimateSystem::ClimateSystem() {
    // Initialize SIMD-aligned data
    temperatures.resize(1024, 20.0f);
    pollution_levels.resize(1024, 0.0f);
    radiation_values.resize(1024, 0.0f);
    
    // Initialize climate grid
    climate_grid.resize(32);
    for (auto& row : climate_grid) {
        row.resize(32);
    }
}

void ClimateSystem::update_climate(float delta) {
    update_climate_simd(delta);
}

void ClimateSystem::simulate_resource_extraction(float delta) {
    for (auto& site : neofuel_sites) {
        float accident_chance = NEOFUEL_ACCIDENT_BASE_CHANCE * 
            (1.0f + site.extraction_rate / 100.0f) *
            (1.0f + site.total_extracted / 10000.0f);
        
        if (UtilityFunctions::randf() < accident_chance) {
            create_industrial_accident(site);
        }
        
        site.total_extracted += site.extraction_rate * delta;
    }
}

float ClimateSystem::get_temperature(const Vector2& position) const {
    size_t x = static_cast<size_t>(position.x) % climate_grid.size();
    size_t y = static_cast<size_t>(position.y) % climate_grid[0].size();
    return climate_grid[x][y].temperature;
}

// ... (implement other getters similarly)

void ClimateSystem::add_hazard(const EnvironmentalHazard& hazard) {
    global_hazards.push_back(hazard);
}

void ClimateSystem::remove_hazard(const Vector2& position, float radius) {
    global_hazards.erase(
        std::remove_if(global_hazards.begin(), global_hazards.end(),
            [&](const EnvironmentalHazard& hazard) {
                return hazard.position.distance_to(position) < radius;
            }
        ),
        global_hazards.end()
    );
}

Array ClimateSystem::get_active_hazards() const {
    Array result;
    for (const auto& hazard : global_hazards) {
        Dictionary hazard_dict;
        hazard_dict["position"] = hazard.position;
        hazard_dict["radius"] = hazard.radius;
        hazard_dict["intensity"] = hazard.intensity;
        hazard_dict["type"] = hazard.type;
        result.push_back(hazard_dict);
    }
    return result;
} 