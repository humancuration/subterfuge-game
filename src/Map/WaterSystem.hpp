#pragma once
#include "ClimateSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <unordered_map>

class WaterSystem {
public:
    // Different types of water bodies and systems
    enum class WaterType {
        GROUNDWATER,
        RIVER,
        LAKE,
        AQUIFER,
        WETLAND
    };

    struct WaterBody {
        alignas(16) godot::Vector2 position;
        alignas(16) float volume;
        alignas(16) float flow_rate;
        WaterType type;
        
        // Health indicators
        float toxicity_level{0.0f};
        float pollution_level{0.0f};
        float biodiversity{1.0f};
        float oxygen_level{1.0f};
        
        // Restoration metrics
        float natural_filtration_rate{0.1f};
        float ecosystem_health{1.0f};
        float restoration_progress{0.0f};
    };

    struct ContaminationFlow {
        alignas(16) godot::Vector2 direction;
        float speed;
        float dispersion_rate;
        std::string contaminant_type;
    };

private:
    std::vector<WaterBody> water_bodies;
    std::vector<ContaminationFlow> contamination_flows;
    
    // SIMD-aligned data for batch processing
    alignas(32) std::vector<float> toxicity_levels;
    alignas(32) std::vector<float> restoration_rates;
    
    // Constants for simulation
    static constexpr float NATURAL_RECOVERY_RATE = 0.001f;  // Base rate of natural water purification
    static constexpr float ECOSYSTEM_IMPACT_THRESHOLD = 0.3f;  // When ecosystem damage begins
    static constexpr float CRITICAL_POLLUTION_LEVEL = 0.7f;  // Emergency response needed
    
    std::unique_ptr<JobSystem> job_system;

public:
    void contaminate_water_supply(const godot::Vector2& source, float radius, const std::string& contaminant_type) {
        // Create contamination flow patterns based on terrain and water table
        std::vector<ContaminationFlow> flows = calculate_contamination_flows(source, radius);
        
        for (const auto& flow : flows) {
            propagate_contamination(flow);
        }
        
        // Trigger adaptation and response systems
        trigger_environmental_response(source, radius);
    }

    void update_water_systems_simd(float delta_time) {
        const size_t body_count = water_bodies.size();
        
        for (size_t i = 0; i < body_count; i += 8) {
            // Process water body updates in SIMD batches
            update_water_batch_simd(i, std::min(size_t(8), body_count - i), delta_time);
        }
        
        process_restoration_effects(delta_time);
    }

private:
    void update_water_batch_simd(size_t start_idx, size_t count, float delta_time) {
        // Load water quality data
        __m256 toxicity = _mm256_load_ps(&toxicity_levels[start_idx]);
        __m256 restoration = _mm256_load_ps(&restoration_rates[start_idx]);
        __m256 delta = _mm256_set1_ps(delta_time);
        
        // Natural recovery calculation
        __m256 recovery = _mm256_mul_ps(
            _mm256_set1_ps(NATURAL_RECOVERY_RATE),
            _mm256_sub_ps(_mm256_set1_ps(1.0f), toxicity)
        );
        
        // Apply restoration effects
        __m256 total_healing = _mm256_add_ps(recovery, restoration);
        toxicity = _mm256_sub_ps(toxicity, _mm256_mul_ps(total_healing, delta));
        
        // Ensure values stay in valid range
        toxicity = _mm256_max_ps(_mm256_setzero_ps(),
            _mm256_min_ps(toxicity, _mm256_set1_ps(1.0f)));
        
        // Store results
        _mm256_store_ps(&toxicity_levels[start_idx], toxicity);
    }

    void process_restoration_effects(float delta_time) {
        for (auto& body : water_bodies) {
            // Natural ecosystem restoration
            if (body.toxicity_level < ECOSYSTEM_IMPACT_THRESHOLD) {
                body.ecosystem_health += delta_time * NATURAL_RECOVERY_RATE * 
                    (1.0f - body.ecosystem_health);
                body.biodiversity += delta_time * body.ecosystem_health * 0.1f;
            }
            
            // Technology-assisted restoration
            if (body.restoration_progress > 0.0f) {
                float restoration_effect = body.restoration_progress * delta_time;
                body.toxicity_level = std::max(0.0f, body.toxicity_level - restoration_effect);
                body.pollution_level = std::max(0.0f, body.pollution_level - restoration_effect);
                
                // Improved natural filtration from restoration efforts
                body.natural_filtration_rate = std::min(1.0f, 
                    body.natural_filtration_rate + restoration_effect * 0.1f);
            }
        }
    }

    void trigger_environmental_response(const godot::Vector2& location, float radius) {
        // Find nearby waypoints that could help with restoration
        auto nearby_waypoints = spatial_system->get_waypoints_in_radius(location, radius * 2.0f);
        
        for (auto* waypoint : nearby_waypoints) {
            // Encourage development of water treatment capabilities
            waypoint->increase_trait_chance("WaterPurification", 0.3f);
            waypoint->increase_trait_chance("EnvironmentalRestoration", 0.4f);
            
            // Potential for new research and technology
            if (waypoint->get_stat("TechnologicalLevel") > 0.6f) {
                waypoint->add_research_project("Advanced Water Filtration");
                waypoint->add_research_project("Ecosystem Restoration");
            }
            
            // Cultural response
            waypoint->modify_stat("EnvironmentalAwareness", 0.2f);
            waypoint->consider_trait("WaterConservation");
        }
    }

    std::vector<ContaminationFlow> calculate_contamination_flows(
        const godot::Vector2& source, float radius) {
        
        std::vector<ContaminationFlow> flows;
        
        // Consider terrain elevation and water table
        auto terrain_data = terrain_system->get_terrain_data(source, radius);
        
        // Calculate flow directions based on elevation gradients
        for (const auto& gradient : terrain_data.elevation_gradients) {
            flows.push_back({
                gradient.direction,
                calculate_flow_speed(gradient.slope, terrain_data.soil_type),
                calculate_dispersion_rate(terrain_data.soil_type),
                "industrial_waste"  // Example contaminant type
            });
        }
        
        return flows;
    }

    float calculate_flow_speed(float slope, uint8_t soil_type) {
        // Different soil types affect flow speed
        float soil_permeability = get_soil_permeability(soil_type);
        return slope * soil_permeability;
    }

    float calculate_dispersion_rate(uint8_t soil_type) {
        // Soil composition affects how contaminants spread
        return get_soil_dispersion_factor(soil_type);
    }
}; 