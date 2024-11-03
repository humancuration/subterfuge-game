#include "AtmosphereSystem.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void AtmosphereSystem::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update_atmosphere", "delta"), &AtmosphereSystem::update_atmosphere);
    ClassDB::bind_method(D_METHOD("get_pollution_level"), &AtmosphereSystem::get_pollution_level);
    ClassDB::bind_method(D_METHOD("get_temperature"), &AtmosphereSystem::get_temperature);
    ClassDB::bind_method(D_METHOD("get_humidity"), &AtmosphereSystem::get_humidity);
    ClassDB::bind_method(D_METHOD("get_co2_level"), &AtmosphereSystem::get_co2_level);
    
    // Properties
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pollution_dispersion_rate"), 
                 "set_pollution_dispersion_rate", "get_pollution_dispersion_rate");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "heat_dissipation_rate"),
                 "set_heat_dissipation_rate", "get_heat_dissipation_rate");
}

AtmosphereSystem::AtmosphereSystem() {
    // Initialize SIMD-aligned data
    pollution_levels.resize(1024, 0.0f);
    temperatures.resize(1024, 20.0f);
    wind_velocities_x.resize(1024, 0.0f);
    wind_velocities_y.resize(1024, 0.0f);
    
    // Initialize atmosphere grid
    atmosphere_grid.resize(32);
    for (auto& row : atmosphere_grid) {
        row.resize(32);
    }
}

void AtmosphereSystem::update_atmosphere(float delta) {
    update_atmosphere_simd(delta);
}

float AtmosphereSystem::get_pollution_level() const {
    // Return average pollution level
    float total = 0.0f;
    for (const auto& level : pollution_levels) {
        total += level;
    }
    return total / pollution_levels.size();
}

float AtmosphereSystem::get_temperature() const {
    // Return average temperature
    float total = 0.0f;
    for (const auto& temp : temperatures) {
        total += temp;
    }
    return total / temperatures.size();
}

float AtmosphereSystem::get_humidity() const {
    float total = 0.0f;
    for (const auto& row : atmosphere_grid) {
        for (const auto& cell : row) {
            total += cell.humidity;
        }
    }
    return total / (atmosphere_grid.size() * atmosphere_grid[0].size());
}

float AtmosphereSystem::get_co2_level() const {
    float total = 0.0f;
    for (const auto& row : atmosphere_grid) {
        for (const auto& cell : row) {
            total += cell.co2_level;
        }
    }
    return total / (atmosphere_grid.size() * atmosphere_grid[0].size());
} 