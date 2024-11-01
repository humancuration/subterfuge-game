#pragma once
#include "WaypointTraitSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <unordered_map>

class EnvironmentalInfluenceSystem {
private:
    struct EnvironmentalFactor {
        alignas(16) float intensity;
        alignas(16) float spread_rate;
        alignas(16) float decay_rate;
        std::string type;
        std::function<void(Waypoint*, float)> influence_effect;
    };

    struct GlobalEnvironment {
        alignas(32) std::vector<float> pollution_levels;
        alignas(32) std::vector<float> resource_depletion;
        alignas(32) std::vector<float> cultural_influence;
        alignas(32) std::vector<float> technological_radiation;
        
        // Regional climate data
        struct ClimateCell {
            float temperature;
            float humidity;
            float air_quality;
            float biodiversity;
        };
        std::vector<ClimateCell> climate_grid;
    };

    // SIMD-aligned environmental data
    alignas(32) std::vector<float> environmental_impacts;
    alignas(32) std::vector<float> waypoint_contributions;
    
    GlobalEnvironment global_environment;
    std::unique_ptr<JobSystem> job_system;

public:
    void process_environmental_changes_simd(float delta_time) {
        // Process global environmental changes in parallel
        const size_t grid_size = global_environment.pollution_levels.size();
        
        for (size_t i = 0; i < grid_size; i += 1024) {
            job_system->schedule_job(
                [this, i, grid_size, delta_time]() {
                    update_environment_batch_simd(i, 
                        std::min(size_t(1024), grid_size - i), 
                        delta_time);
                },
                JobSystem::Priority::HIGH
            );
        }
        
        job_system->process_jobs();
    }

    void register_waypoint_influence(Waypoint* waypoint) {
        // Calculate waypoint's environmental impact
        auto impact = calculate_waypoint_impact_simd(waypoint);
        
        // Propagate influence to nearby areas
        propagate_environmental_influence(waypoint, impact);
    }

private:
    void update_environment_batch_simd(size_t start_idx, size_t count, float delta_time) {
        // Process environmental factors using SIMD
        for (size_t i = 0; i < count; i += 8) {
            // Load environmental data
            __m256 pollution = _mm256_load_ps(&global_environment.pollution_levels[start_idx + i]);
            __m256 resources = _mm256_load_ps(&global_environment.resource_depletion[start_idx + i]);
            __m256 culture = _mm256_load_ps(&global_environment.cultural_influence[start_idx + i]);
            __m256 tech = _mm256_load_ps(&global_environment.technological_radiation[start_idx + i]);
            
            // Calculate combined environmental impact
            __m256 impact = calculate_environmental_impact_simd(
                pollution, resources, culture, tech
            );
            
            // Apply feedback effects
            apply_environmental_feedback_simd(start_idx + i, impact, delta_time);
        }
    }

    void propagate_environmental_influence(Waypoint* source, const EnvironmentalImpact& impact) {
        // Get nearby waypoints using spatial partitioning
        auto nearby_waypoints = get_nearby_waypoints(source->get_position(), impact.radius);
        
        for (auto* waypoint : nearby_waypoints) {
            // Calculate influence based on distance and source strength
            float influence = calculate_influence_falloff_simd(
                source->get_position(),
                waypoint->get_position(),
                impact.strength
            );
            
            // Apply influence effects
            if (influence > 0.1f) {  // Threshold for minimal influence
                apply_environmental_effects(waypoint, impact.type, influence);
            }
        }
    }

    void apply_environmental_effects(Waypoint* waypoint, const std::string& effect_type, float intensity) {
        // Example effects:
        if (effect_type == "Industrial") {
            // Increase pollution, decrease environmental health
            waypoint->modify_stat("EnvironmentalHealth", -intensity * 0.5f);
            waypoint->add_resource_bonus("Production", intensity * 0.3f);
            
            // Might trigger adaptation traits
            if (intensity > 0.7f) {
                waypoint->consider_trait("PollutionResistance");
            }
        }
        else if (effect_type == "Cultural") {
            // Spread cultural influence
            waypoint->modify_stat("CulturalDevelopment", intensity * 0.4f);
            waypoint->increase_trait_chance("ArtisticExpression", intensity);
        }
        else if (effect_type == "Technological") {
            // Spread technological advancement
            waypoint->modify_stat("TechnologicalLevel", intensity * 0.3f);
            waypoint->increase_research_speed(intensity * 0.2f);
            
            // Might enable new capabilities
            if (intensity > 0.8f) {
                waypoint->unlock_tech_potential("AdvancedResearch");
            }
        }
    }

    __m256 calculate_environmental_impact_simd(
        __m256 pollution, __m256 resources, 
        __m256 culture, __m256 tech) {
            
        // Combine factors with different weights
        __m256 weights = _mm256_set_ps(0.3f, 0.25f, 0.25f, 0.2f, 
                                      0.3f, 0.25f, 0.25f, 0.2f);
        
        __m256 weighted_pollution = _mm256_mul_ps(pollution, weights);
        __m256 weighted_resources = _mm256_mul_ps(resources, weights);
        __m256 weighted_culture = _mm256_mul_ps(culture, weights);
        __m256 weighted_tech = _mm256_mul_ps(tech, weights);
        
        // Combine all factors
        return _mm256_add_ps(
            _mm256_add_ps(weighted_pollution, weighted_resources),
            _mm256_add_ps(weighted_culture, weighted_tech)
        );
    }
}; 