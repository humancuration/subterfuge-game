#pragma once
#include "OceanSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <vector>

class MarineEcosystemSystem {
private:
    struct MarineSpecies {
        enum class Type {
            PHYTOPLANKTON,
            ZOOPLANKTON,
            KRILL,
            SMALL_FISH,
            PREDATORY_FISH,
            MARINE_MAMMALS,
            CORAL,
            SHELLFISH
        };

        Type type;
        float population{1.0f};
        float reproduction_rate;
        float mortality_rate;
        float migration_factor;
        std::vector<Type> prey_types;
        std::vector<Type> predator_types;
        bool is_endangered{false};
    };

    struct FoodWeb {
        struct Relationship {
            float predation_rate;
            float energy_transfer;
            bool is_critical_dependency;
        };
        
        std::unordered_map<
            std::pair<MarineSpecies::Type, MarineSpecies::Type>,
            Relationship,
            PairHash
        > relationships;
    };

    struct HumanActivity {
        enum class Type {
            INDUSTRIAL_FISHING,
            TRADITIONAL_FISHING,
            AQUACULTURE,
            SHIPPING,
            COASTAL_DEVELOPMENT,
            WASTE_DISPOSAL,
            CONSERVATION
        };

        Type activity_type;
        float intensity;
        float sustainability_rating;
        std::vector<MarineSpecies::Type> affected_species;
    };

    struct MarineEvent {
        enum class Type {
            ALGAL_BLOOM,
            CORAL_BLEACHING,
            FISH_MIGRATION,
            MASS_SPAWNING,
            UPWELLING,
            DEAD_ZONE_FORMATION,
            SPECIES_RECOVERY
        };

        Type type;
        float intensity;
        float duration;
        godot::Vector2 location;
        std::vector<MarineSpecies::Type> affected_species;
    };

    // SIMD-aligned ecosystem data
    alignas(32) std::vector<float> species_populations;
    alignas(32) std::vector<float> reproduction_rates;
    alignas(32) std::vector<float> mortality_rates;
    
    std::vector<MarineSpecies> marine_species;
    std::vector<HumanActivity> current_activities;
    std::vector<MarineEvent> active_events;
    FoodWeb food_web;

public:
    void update_ecosystem_simd(float delta_time, const OceanSystem& ocean) {
        process_species_interactions_simd(delta_time);
        process_human_impacts(delta_time);
        check_ecosystem_events(ocean);
        update_migration_patterns(ocean);
    }

private:
    void process_species_interactions_simd(float delta_time) {
        const size_t species_count = marine_species.size();
        
        for (size_t i = 0; i < species_count; i += 8) {
            // Load species data
            __m256 populations = _mm256_load_ps(&species_populations[i]);
            __m256 reproduction = _mm256_load_ps(&reproduction_rates[i]);
            __m256 mortality = _mm256_load_ps(&mortality_rates[i]);
            
            // Calculate population changes
            __m256 growth = _mm256_mul_ps(populations, reproduction);
            __m256 death = _mm256_mul_ps(populations, mortality);
            
            // Update populations
            populations = _mm256_add_ps(
                populations,
                _mm256_mul_ps(
                    _mm256_sub_ps(growth, death),
                    _mm256_set1_ps(delta_time)
                )
            );
            
            // Store results
            _mm256_store_ps(&species_populations[i], populations);
        }
    }

    void process_human_impacts(float delta_time) {
        for (const auto& activity : current_activities) {
            switch (activity.activity_type) {
                case HumanActivity::Type::INDUSTRIAL_FISHING:
                    process_industrial_fishing(activity, delta_time);
                    break;
                case HumanActivity::Type::TRADITIONAL_FISHING:
                    process_traditional_fishing(activity, delta_time);
                    break;
                case HumanActivity::Type::CONSERVATION:
                    apply_conservation_effects(activity, delta_time);
                    break;
            }
        }
    }

    void process_industrial_fishing(const HumanActivity& activity, float delta_time) {
        float impact = activity.intensity * (1.0f - activity.sustainability_rating);
        
        for (const auto& species_type : activity.affected_species) {
            size_t species_idx = get_species_index(species_type);
            
            // Direct fishing impact
            species_populations[species_idx] *= (1.0f - impact * delta_time);
            
            // Bycatch impact
            float bycatch_rate = calculate_bycatch_rate(species_type, activity.intensity);
            apply_bycatch_impact(species_idx, bycatch_rate, delta_time);
            
            // Habitat damage from trawling
            if (activity.intensity > 0.7f) {
                damage_marine_habitat(species_type, activity.intensity, delta_time);
            }
        }
    }

    void check_ecosystem_events(const OceanSystem& ocean) {
        // Check for algal bloom conditions
        if (check_algal_bloom_conditions(ocean)) {
            trigger_algal_bloom();
        }
        
        // Check for coral bleaching
        if (check_coral_bleaching_conditions(ocean)) {
            trigger_coral_bleaching();
        }
        
        // Check for dead zone formation
        if (check_dead_zone_conditions(ocean)) {
            create_dead_zone();
        }
    }

    bool check_algal_bloom_conditions(const OceanSystem& ocean) {
        // Check water temperature, nutrient levels, and sunlight
        return ocean.get_temperature() > 25.0f &&
               ocean.get_nutrient_level() > 0.8f &&
               ocean.get_sunlight_intensity() > 0.7f;
    }

    void trigger_algal_bloom() {
        MarineEvent bloom{
            MarineEvent::Type::ALGAL_BLOOM,
            1.0f,
            7.0f, // 7 days duration
            current_location,
            {
                MarineSpecies::Type::PHYTOPLANKTON,
                MarineSpecies::Type::ZOOPLANKTON,
                MarineSpecies::Type::FISH
            }
        };
        
        active_events.push_back(bloom);
        
        // Immediate effects
        increase_phytoplankton_population(2.0f);
        decrease_oxygen_levels(0.5f);
    }

    void update_migration_patterns(const OceanSystem& ocean) {
        for (size_t i = 0; i < marine_species.size(); ++i) {
            if (marine_species[i].migration_factor > 0.0f) {
                calculate_migration_simd(i, ocean);
            }
        }
    }

    void calculate_migration_simd(size_t species_idx, const OceanSystem& ocean) {
        // Load environmental factors
        __m256 temperature = _mm256_load_ps(&ocean.temperatures[0]);
        __m256 food_availability = _mm256_load_ps(&species_populations[0]);
        
        // Calculate migration attractiveness
        __m256 attractiveness = calculate_habitat_attractiveness_simd(
            temperature,
            food_availability,
            marine_species[species_idx].type
        );
        
        // Update population distribution
        update_species_distribution_simd(species_idx, attractiveness);
    }
}; 