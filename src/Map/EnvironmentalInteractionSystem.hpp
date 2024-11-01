#pragma once
#include "AtmosphereSystem.hpp"
#include "WaterSystem.hpp"
#include "SoilSystem.hpp"
#include "../Core/JobSystem.hpp"

class EnvironmentalInteractionSystem {
private:
    struct ChemicalInteraction {
        alignas(16) float reaction_rate;
        alignas(16) float concentration;
        std::string reagent_type;
        std::string product_type;
        bool is_catalyzed;
    };

    struct NutrientFlow {
        alignas(16) godot::Vector2 direction;
        float transport_rate;
        std::string nutrient_type;
        std::vector<std::string> affected_biomes;
    };

    struct AtmosphericEvent {
        enum class Type {
            ACID_RAIN,
            DUST_STORM,
            NUTRIENT_TRANSPORT,
            CHEMICAL_PRECIPITATION,
            THERMAL_INVERSION,
            SMOG_FORMATION
        };

        Type type;
        float intensity;
        float duration;
        std::vector<ChemicalInteraction> chemical_reactions;
    };

    // SIMD-aligned interaction data
    alignas(32) std::vector<float> reaction_rates;
    alignas(32) std::vector<float> transport_velocities;
    alignas(32) std::vector<float> concentration_levels;

    std::vector<AtmosphericEvent> active_events;
    std::vector<NutrientFlow> nutrient_flows;
    
    // References to other systems
    AtmosphereSystem& atmosphere;
    WaterSystem& water;
    SoilSystem& soil;

public:
    void process_environmental_interactions(float delta_time) {
        // Process chemical reactions in parallel
        process_chemical_reactions_simd(delta_time);
        
        // Update nutrient transport
        update_nutrient_flows_simd(delta_time);
        
        // Handle atmospheric events
        process_atmospheric_events(delta_time);
        
        // Check for new event formation
        check_event_conditions();
    }

private:
    void process_chemical_reactions_simd(float delta_time) {
        const size_t reaction_count = reaction_rates.size();
        
        for (size_t i = 0; i < reaction_count; i += 8) {
            __m256 rates = _mm256_load_ps(&reaction_rates[i]);
            __m256 concentrations = _mm256_load_ps(&concentration_levels[i]);
            __m256 delta = _mm256_set1_ps(delta_time);
            
            // Calculate reaction progress
            __m256 progress = _mm256_mul_ps(
                _mm256_mul_ps(rates, concentrations),
                delta
            );
            
            // Update concentrations
            concentrations = _mm256_sub_ps(concentrations, progress);
            _mm256_store_ps(&concentration_levels[i], concentrations);
        }
    }

    void check_event_conditions() {
        // Check for acid rain formation
        if (check_acid_rain_conditions()) {
            create_acid_rain_event();
        }
        
        // Check for dust storm conditions
        if (check_dust_storm_conditions()) {
            create_dust_storm_event();
        }
        
        // Check for thermal inversions
        if (check_thermal_inversion_conditions()) {
            create_thermal_inversion_event();
        }
    }

    bool check_acid_rain_conditions() {
        // Example conditions for acid rain formation:
        // High sulfur dioxide + high nitrogen oxides + high humidity
        const float SO2_THRESHOLD = 0.5f;
        const float NOX_THRESHOLD = 0.4f;
        const float HUMIDITY_THRESHOLD = 0.7f;
        
        return atmosphere.get_pollutant_level("SO2") > SO2_THRESHOLD &&
               atmosphere.get_pollutant_level("NOx") > NOX_THRESHOLD &&
               atmosphere.get_humidity() > HUMIDITY_THRESHOLD;
    }

    void create_acid_rain_event() {
        AtmosphericEvent event{
            AtmosphericEvent::Type::ACID_RAIN,
            calculate_acid_rain_intensity(),
            calculate_event_duration(),
            generate_acid_rain_reactions()
        };
        
        active_events.push_back(event);
        
        // Apply immediate effects
        apply_acid_rain_effects(event);
    }

    void apply_acid_rain_effects(const AtmosphericEvent& event) {
        // Affect soil pH
        soil.modify_ph_levels(event.intensity * -0.5f);
        
        // Affect water bodies
        water.add_contamination("acid", event.intensity);
        
        // Damage vegetation
        affect_vegetation(event.intensity);
        
        // Create runoff effects
        create_acidic_runoff(event.intensity);
    }

    void update_nutrient_flows_simd(float delta_time) {
        const size_t flow_count = nutrient_flows.size();
        
        for (size_t i = 0; i < flow_count; i += 8) {
            __m256 velocities = _mm256_load_ps(&transport_velocities[i]);
            __m256 delta = _mm256_set1_ps(delta_time);
            
            // Calculate transport distances
            __m256 distances = _mm256_mul_ps(velocities, delta);
            
            // Update nutrient positions and apply effects
            update_nutrient_positions_simd(i, distances);
        }
    }

    void create_dust_storm_event() {
        // Example: Saharan dust reaching Amazon
        if (check_wind_patterns_favorable()) {
            NutrientFlow flow{
                calculate_wind_direction(),
                calculate_transport_rate(),
                "phosphorus",
                {"rainforest", "coastal"}
            };
            
            nutrient_flows.push_back(flow);
            
            // Start tracking the nutrient transport
            track_nutrient_transport(flow);
        }
    }

    void track_nutrient_transport(const NutrientFlow& flow) {
        // Track the movement of nutrients and their effects
        for (const auto& biome : flow.affected_biomes) {
            // Increase soil fertility in affected areas
            soil.modify_fertility(biome, flow.transport_rate * 0.1f);
            
            // Affect local water bodies
            if (biome == "coastal") {
                water.add_nutrients(flow.nutrient_type, flow.transport_rate);
                // This might trigger algal blooms or other effects
            }
        }
    }
}; 