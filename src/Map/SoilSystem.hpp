#pragma once
#include "WaterSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <vector>

class SoilSystem {
private:
    struct SoilCell {
        alignas(16) float contamination_level{0.0f};
        alignas(16) float fertility{1.0f};
        alignas(16) float moisture{0.5f};
        
        // Soil composition
        float organic_matter{0.05f};
        float mineral_content{0.95f};
        float microbe_activity{1.0f};
        
        // Chemical properties
        float ph_level{7.0f};
        float nitrogen_level{0.0f};
        float phosphorus_level{0.0f};
        float heavy_metals{0.0f};
        
        // Physical properties
        float compaction{0.0f};
        float erosion_risk{0.0f};
    };

    struct ContaminantFlow {
        alignas(16) godot::Vector2 direction;
        float speed;
        float absorption_rate;
        std::string contaminant_type;
    };

    // SIMD-aligned soil data
    alignas(32) std::vector<float> contamination_levels;
    alignas(32) std::vector<float> fertility_values;
    alignas(32) std::vector<float> moisture_levels;
    
    std::vector<std::vector<SoilCell>> soil_grid;
    std::vector<ContaminantFlow> contaminant_flows;

public:
    void update_soil_conditions_simd(float delta_time, const WaterSystem& water_system) {
        const size_t grid_size = soil_grid.size() * soil_grid[0].size();
        
        for (size_t i = 0; i < grid_size; i += 1024) {
            job_system->schedule_job(
                [this, i, grid_size, delta_time, &water_system]() {
                    update_soil_batch_simd(i, 
                        std::min(size_t(1024), grid_size - i), 
                        delta_time, water_system);
                },
                JobSystem::Priority::HIGH
            );
        }
        
        job_system->process_jobs();
        process_contaminant_flows(delta_time);
    }

    void apply_industrial_impact(const godot::Vector2& location, float radius, 
                               const std::string& contaminant_type) {
        // Create contamination flows based on soil type and slope
        auto flows = calculate_contamination_flows(location, radius);
        
        for (const auto& flow : flows) {
            propagate_contamination(flow);
        }
        
        // Update soil properties
        update_soil_properties(location, radius);
    }

private:
    void update_soil_batch_simd(size_t start_idx, size_t count, float delta_time,
                               const WaterSystem& water_system) {
        for (size_t i = 0; i < count; i += 8) {
            __m256 contamination = _mm256_load_ps(&contamination_levels[start_idx + i]);
            __m256 fertility = _mm256_load_ps(&fertility_values[start_idx + i]);
            __m256 moisture = _mm256_load_ps(&moisture_levels[start_idx + i]);
            
            // Calculate natural recovery
            __m256 recovery = _mm256_mul_ps(
                _mm256_set1_ps(0.1f),
                _mm256_sub_ps(_mm256_set1_ps(1.0f), contamination)
            );
            
            // Update values
            contamination = _mm256_sub_ps(contamination, recovery);
            fertility = _mm256_add_ps(fertility, recovery);
            
            // Store results
            _mm256_store_ps(&contamination_levels[start_idx + i], contamination);
            _mm256_store_ps(&fertility_values[start_idx + i], fertility);
        }
    }

    void process_contaminant_flows(float delta_time) {
        // Process contaminant movement through soil
        for (const auto& flow : contaminant_flows) {
            update_contaminant_flow_simd(flow, delta_time);
        }
    }
}; 