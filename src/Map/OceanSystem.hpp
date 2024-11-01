#pragma once
#include "ClimateSystem.hpp"
#include "AtmosphereSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <vector>

class OceanSystem {
private:
    struct OceanCell {
        alignas(16) float temperature;
        alignas(16) float salinity;
        alignas(16) float ph_level;
        alignas(16) godot::Vector2 current_velocity;
        
        // Chemical composition
        float dissolved_co2;
        float dissolved_oxygen;
        float carbonate_concentration;
        
        // Marine ecosystem health
        float phytoplankton_density;
        float coral_health;
        float biodiversity;
        
        // Physical properties
        float depth;
        float pressure;
        float turbidity;
        float surface_roughness;
    };

    struct CurrentSystem {
        enum class Type {
            SURFACE,        // Like Gulf Stream
            DEEP_WATER,     // Thermohaline circulation
            UPWELLING,      // Coastal upwelling
            GYRE           // Circular ocean currents
        };

        Type type;
        float strength;
        float temperature_influence;
        std::vector<godot::Vector2> path_points;
        bool is_seasonal;
    };

    struct ThermalLayer {
        float depth_start;
        float depth_end;
        float temperature_gradient;
        bool is_thermocline;
    };

    // SIMD-aligned ocean data
    alignas(32) std::vector<float> temperatures;
    alignas(32) std::vector<float> salinities;
    alignas(32) std::vector<float> ph_levels;
    alignas(32) std::vector<float> current_velocities_x;
    alignas(32) std::vector<float> current_velocities_y;
    
    std::vector<std::vector<OceanCell>> ocean_grid;
    std::vector<CurrentSystem> ocean_currents;
    std::vector<ThermalLayer> thermal_layers;
    
    // Constants for simulation
    static constexpr float THERMAL_EXPANSION_COEFFICIENT = 0.0002f;  // Per degree Celsius
    static constexpr float CO2_ABSORPTION_RATE = 0.000001f;         // Per second
    static constexpr float CORAL_BLEACHING_THRESHOLD = 29.0f;       // Celsius
    static constexpr float MIN_PH_FOR_CALCIFICATION = 7.8f;         // pH level

public:
    void update_ocean_systems_simd(float delta_time, const AtmosphereSystem& atmosphere) {
        process_thermal_dynamics(delta_time);
        update_currents_simd(delta_time);
        process_chemical_exchange(delta_time, atmosphere);
        update_marine_ecosystems(delta_time);
    }

private:
    void process_thermal_dynamics(float delta_time) {
        const size_t cell_count = ocean_grid.size() * ocean_grid[0].size();
        
        for (size_t i = 0; i < cell_count; i += 8) {
            __m256 temps = _mm256_load_ps(&temperatures[i]);
            __m256 depths = _mm256_load_ps(&ocean_grid[i / ocean_grid[0].size()][i % ocean_grid[0].size()].depth);
            
            // Calculate thermal stratification
            __m256 thermal_effect = calculate_thermal_stratification_simd(temps, depths);
            
            // Apply thermal expansion
            __m256 expansion = _mm256_mul_ps(
                thermal_effect,
                _mm256_set1_ps(THERMAL_EXPANSION_COEFFICIENT)
            );
            
            // Update temperatures
            temps = _mm256_add_ps(temps, expansion);
            _mm256_store_ps(&temperatures[i], temps);
        }
    }

    void update_currents_simd(float delta_time) {
        for (auto& current : ocean_currents) {
            // Process current dynamics based on type
            switch (current.type) {
                case CurrentSystem::Type::SURFACE:
                    process_surface_current_simd(current, delta_time);
                    break;
                case CurrentSystem::Type::DEEP_WATER:
                    process_thermohaline_circulation_simd(current, delta_time);
                    break;
                case CurrentSystem::Type::UPWELLING:
                    process_upwelling_simd(current, delta_time);
                    break;
                case CurrentSystem::Type::GYRE:
                    process_gyre_dynamics_simd(current, delta_time);
                    break;
            }
        }
    }

    void process_chemical_exchange(float delta_time, const AtmosphereSystem& atmosphere) {
        const size_t cell_count = ocean_grid.size() * ocean_grid[0].size();
        
        for (size_t i = 0; i < cell_count; i += 8) {
            // Load ocean chemistry data
            __m256 ph = _mm256_load_ps(&ph_levels[i]);
            __m256 co2 = _mm256_load_ps(&ocean_grid[i / ocean_grid[0].size()][i % ocean_grid[0].size()].dissolved_co2);
            
            // Calculate CO2 absorption based on atmospheric conditions
            __m256 atmospheric_co2 = _mm256_set1_ps(atmosphere.get_co2_level());
            __m256 absorption = calculate_co2_absorption_simd(ph, co2, atmospheric_co2);
            
            // Update ocean chemistry
            co2 = _mm256_add_ps(co2, absorption);
            ph = update_ph_levels_simd(ph, co2);
            
            // Store results
            _mm256_store_ps(&ph_levels[i], ph);
            _mm256_store_ps(&ocean_grid[i / ocean_grid[0].size()][i % ocean_grid[0].size()].dissolved_co2, co2);
        }
    }

    void update_marine_ecosystems(float delta_time) {
        const size_t cell_count = ocean_grid.size() * ocean_grid[0].size();
        
        for (size_t i = 0; i < cell_count; i += 8) {
            auto& cells = ocean_grid[i / ocean_grid[0].size()];
            
            // Update coral health based on temperature and pH
            __m256 temps = _mm256_load_ps(&temperatures[i]);
            __m256 ph = _mm256_load_ps(&ph_levels[i]);
            
            __m256 coral_stress = calculate_coral_stress_simd(temps, ph);
            
            // Update phytoplankton based on nutrients and light
            __m256 phytoplankton = calculate_phytoplankton_growth_simd(
                temps,
                _mm256_load_ps(&cells[i % ocean_grid[0].size()].dissolved_oxygen),
                _mm256_load_ps(&cells[i % ocean_grid[0].size()].turbidity)
            );
            
            // Store ecosystem updates
            for (size_t j = 0; j < 8 && i + j < cell_count; ++j) {
                auto& cell = cells[(i + j) % ocean_grid[0].size()];
                cell.coral_health = std::max(0.0f, cell.coral_health - coral_stress[j]);
                cell.phytoplankton_density = phytoplankton[j];
                cell.biodiversity = calculate_biodiversity(cell);
            }
        }
    }

    float calculate_biodiversity(const OceanCell& cell) {
        // Complex calculation based on multiple factors
        float chemical_factor = (cell.ph_level - 7.0f) / 1.5f;  // Optimal pH around 8.5
        float temperature_factor = 1.0f - std::abs(cell.temperature - 20.0f) / 15.0f;
        float oxygen_factor = cell.dissolved_oxygen / 10.0f;
        
        return (chemical_factor + temperature_factor + oxygen_factor) / 3.0f *
               cell.coral_health * cell.phytoplankton_density;
    }

    void process_gyre_dynamics_simd(CurrentSystem& gyre, float delta_time) {
        // Simulate large-scale circular ocean currents
        const size_t point_count = gyre.path_points.size();
        
        alignas(32) float velocities_x[8];
        alignas(32) float velocities_y[8];
        
        for (size_t i = 0; i < point_count; i += 8) {
            // Calculate Coriolis effect and wind stress
            __m256 coriolis = calculate_coriolis_force_simd(
                _mm256_load_ps(&gyre.path_points[i].x),
                _mm256_load_ps(&gyre.path_points[i].y)
            );
            
            // Update gyre velocities
            __m256 vel_x = _mm256_load_ps(velocities_x);
            __m256 vel_y = _mm256_load_ps(velocities_y);
            
            vel_x = _mm256_add_ps(vel_x, coriolis);
            vel_y = _mm256_add_ps(vel_y, _mm256_mul_ps(coriolis, _mm256_set1_ps(-1.0f)));
            
            _mm256_store_ps(velocities_x, vel_x);
            _mm256_store_ps(velocities_y, vel_y);
        }
    }
}; 