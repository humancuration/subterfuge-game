#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "../Core/JobSystem.hpp"
#include <vector>

class AtmosphereSystem : public godot::Node3D {
    GDCLASS(AtmosphereSystem, Node3D)

private:
    struct AirCell {
        alignas(16) float pollution_level{0.0f};
        alignas(16) float temperature{20.0f};
        alignas(16) float humidity{0.5f};
        alignas(16) godot::Vector2 wind_velocity;
        
        // Chemical composition
        float co2_level{400.0f};  // ppm
        float methane_level{1.8f}; // ppm
        float particulate_matter{0.0f};
        
        // Radiation and heat
        float thermal_radiation{0.0f};
        float industrial_heat{0.0f};
        float urban_heat_effect{0.0f};
    };

    struct WindPattern {
        alignas(16) godot::Vector2 direction;
        float strength;
        float turbulence;
    };

    // SIMD-aligned atmospheric data
    alignas(32) std::vector<float> pollution_levels;
    alignas(32) std::vector<float> temperatures;
    alignas(32) std::vector<float> wind_velocities_x;
    alignas(32) std::vector<float> wind_velocities_y;
    
    std::vector<std::vector<AirCell>> atmosphere_grid;
    std::vector<WindPattern> wind_patterns;
    
    static constexpr float POLLUTION_DISPERSION_RATE = 0.1f;
    static constexpr float HEAT_DISSIPATION_RATE = 0.05f;

protected:
    static void _bind_methods();

public:
    AtmosphereSystem();
    
    void update_atmosphere(float delta);
    float get_pollution_level() const;
    float get_temperature() const;
    float get_humidity() const;
    float get_co2_level() const;
    
    // Property setters/getters
    void set_pollution_dispersion_rate(float rate) { POLLUTION_DISPERSION_RATE = rate; }
    float get_pollution_dispersion_rate() const { return POLLUTION_DISPERSION_RATE; }
    
    void set_heat_dissipation_rate(float rate) { HEAT_DISSIPATION_RATE = rate; }
    float get_heat_dissipation_rate() const { return HEAT_DISSIPATION_RATE; }

    // Original functionality restored
    void update_atmosphere_simd(float delta_time) {
        const size_t grid_size = atmosphere_grid.size() * atmosphere_grid[0].size();
        
        for (size_t i = 0; i < grid_size; i += 1024) {
            job_system->schedule_job(
                [this, i, grid_size, delta_time]() {
                    update_air_batch_simd(i, 
                        std::min(size_t(1024), grid_size - i), 
                        delta_time);
                },
                JobSystem::Priority::HIGH
            );
        }
        
        job_system->process_jobs();
        process_wind_patterns(delta_time);
    }

private:
    void update_air_batch_simd(size_t start_idx, size_t count, float delta_time) {
        for (size_t i = 0; i < count; i += 8) {
            __m256 pollution = _mm256_load_ps(&pollution_levels[start_idx + i]);
            __m256 temp = _mm256_load_ps(&temperatures[start_idx + i]);
            
            // Calculate pollution dispersion
            __m256 dispersion = _mm256_mul_ps(
                pollution,
                _mm256_set1_ps(POLLUTION_DISPERSION_RATE * delta_time)
            );
            
            // Update values
            pollution = _mm256_sub_ps(pollution, dispersion);
            
            // Store results
            _mm256_store_ps(&pollution_levels[start_idx + i], pollution);
        }
    }

    void process_wind_patterns(float delta_time) {
        // Simulate wind effects on pollution and temperature distribution
        for (const auto& pattern : wind_patterns) {
            apply_wind_effect_simd(pattern, delta_time);
        }
    }

    void apply_wind_effect_simd(const WindPattern& pattern, float delta_time) {
        // Apply wind effects using SIMD operations
        const size_t grid_size = atmosphere_grid.size() * atmosphere_grid[0].size();
        
        for (size_t i = 0; i < grid_size; i += 8) {
            __m256 wind_x = _mm256_load_ps(&wind_velocities_x[i]);
            __m256 wind_y = _mm256_load_ps(&wind_velocities_y[i]);
            
            // Apply wind pattern influence
            __m256 pattern_strength = _mm256_set1_ps(pattern.strength * delta_time);
            __m256 pattern_x = _mm256_set1_ps(pattern.direction.x);
            __m256 pattern_y = _mm256_set1_ps(pattern.direction.y);
            
            wind_x = _mm256_add_ps(wind_x, _mm256_mul_ps(pattern_x, pattern_strength));
            wind_y = _mm256_add_ps(wind_y, _mm256_mul_ps(pattern_y, pattern_strength));
            
            _mm256_store_ps(&wind_velocities_x[i], wind_x);
            _mm256_store_ps(&wind_velocities_y[i], wind_y);
        }
    }
}; 