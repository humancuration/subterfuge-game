#pragma once
#include "EnvironmentalInfluenceSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <unordered_map>

class ClimateSystem {
public:
    // Our "Spice-like" resource
    struct NeoFuel {
        enum class Type {
            RAW,            // Freshly extracted
            REFINED,        // Processed for use
            WASTE,         // Toxic byproduct
            CRYSTALLIZED   // Dangerous mutated form
        };

        float radiation_level;
        float toxicity;
        float instability;
        Type type;
        bool is_volatile;
    };

    struct EnvironmentalHazard {
        alignas(16) godot::Vector2 position;
        alignas(16) float radius;
        alignas(16) float intensity;
        std::string type;
        float decay_rate;
        bool is_permanent;
        
        std::function<void(Waypoint*, float)> hazard_effect;
    };

    struct ClimateCell {
        // Atmospheric conditions
        alignas(16) float temperature;
        alignas(16) float humidity;
        alignas(16) float air_quality;
        alignas(16) float wind_speed;
        godot::Vector2 wind_direction;

        // Water system
        float ground_water_quality;
        float surface_water_quality;
        float water_table_level;
        
        // Pollution tracking
        float air_pollution;
        float soil_contamination;
        float water_pollution;
        float radiation_level;
        
        // Resource deposits
        float neofuel_concentration;
        float rare_earth_minerals;
        float renewable_potential;  // Solar/Wind/etc. viability
        
        // Ecosystem health
        float biodiversity;
        float soil_fertility;
        float forest_density;
        
        std::vector<EnvironmentalHazard> active_hazards;
    };

private:
    // SIMD-aligned climate data
    alignas(32) std::vector<float> temperatures;
    alignas(32) std::vector<float> pollution_levels;
    alignas(32) std::vector<float> radiation_values;
    
    std::vector<std::vector<ClimateCell>> climate_grid;
    std::vector<EnvironmentalHazard> global_hazards;
    
    // Resource extraction tracking
    struct ExtractionSite {
        godot::Vector2 position;
        float extraction_rate;
        float total_extracted;
        float accident_probability;
        std::vector<EnvironmentalHazard> local_hazards;
    };
    
    std::vector<ExtractionSite> neofuel_sites;
    
    // Constants for simulation
    static constexpr float NEOFUEL_ACCIDENT_BASE_CHANCE = 0.001f;  // 0.1% per update
    static constexpr float NEOFUEL_MUTATION_CHANCE = 0.005f;      // 0.5% chance of crystallization
    static constexpr float WATER_CONTAMINATION_RADIUS = 100.0f;
    static constexpr float RADIATION_SPREAD_RATE = 0.1f;

public:
    void update_climate_simd(float delta_time) {
        const size_t grid_size = climate_grid.size() * climate_grid[0].size();
        
        for (size_t i = 0; i < grid_size; i += 1024) {
            job_system->schedule_job(
                [this, i, grid_size, delta_time]() {
                    update_climate_batch_simd(i, 
                        std::min(size_t(1024), grid_size - i), 
                        delta_time);
                },
                JobSystem::Priority::HIGH
            );
        }
        
        job_system->process_jobs();
        process_hazard_effects();
    }

    void simulate_resource_extraction(float delta_time) {
        for (auto& site : neofuel_sites) {
            // Calculate accident probability based on extraction rate and total extracted
            float accident_chance = NEOFUEL_ACCIDENT_BASE_CHANCE * 
                (1.0f + site.extraction_rate / 100.0f) *
                (1.0f + site.total_extracted / 10000.0f);
            
            if (random_chance() < accident_chance) {
                create_industrial_accident(site);
            }

            // Update environmental impact
            update_extraction_impact(site, delta_time);
        }
    }

private:
    void create_industrial_accident(const ExtractionSite& site) {
        EnvironmentalHazard hazard{
            site.position,
            50.0f + random_float() * 150.0f,  // Random radius between 50-200
            1.0f,
            "NeoFuel_Spill",
            0.001f,  // Very slow decay
            false,
            [](Waypoint* w, float intensity) {
                w->modify_stat("EnvironmentalHealth", -intensity * 2.0f);
                w->modify_stat("PopulationHealth", -intensity * 1.5f);
                w->add_status_effect("Contamination");
                
                // Chance to trigger adaptation traits
                if (intensity > 0.7f) {
                    w->consider_trait("RadiationResistance");
                    w->consider_trait("ToxinFiltering");
                }
            }
        };
        
        global_hazards.push_back(hazard);
        
        // Contaminate water supply
        contaminate_water_supply(site.position, WATER_CONTAMINATION_RADIUS);
    }

    void update_extraction_impact(ExtractionSite& site, float delta_time) {
        // Calculate environmental degradation
        float impact_radius = 20.0f + site.total_extracted / 1000.0f;
        float impact_intensity = site.extraction_rate * delta_time;
        
        // Apply impact to nearby climate cells
        apply_extraction_impact(site.position, impact_radius, impact_intensity);
        
        // Check for NeoFuel crystallization
        if (random_chance() < NEOFUEL_MUTATION_CHANCE * site.extraction_rate) {
            create_crystal_formation(site);
        }
        
        site.total_extracted += site.extraction_rate * delta_time;
    }

    void create_crystal_formation(const ExtractionSite& site) {
        EnvironmentalHazard crystal{
            site.position + random_offset(10.0f),
            5.0f + random_float() * 10.0f,
            2.0f,
            "NeoFuel_Crystal",
            0.0f,  // Permanent
            true,
            [](Waypoint* w, float intensity) {
                w->modify_stat("EnvironmentalHealth", -intensity * 3.0f);
                w->modify_stat("PopulationHealth", -intensity * 2.0f);
                w->add_status_effect("Mutation");
                w->increase_trait_chance("CrystalResistance", 0.2f);
            }
        };
        
        site.local_hazards.push_back(crystal);
    }

    void contaminate_water_supply(const godot::Vector2& position, float radius) {
        // Implement water table contamination that spreads over time
        // This could affect multiple climate cells and persist
    }

    void process_hazard_effects() {
        // Process both global and local hazards
        // Use spatial partitioning to efficiently find affected waypoints
    }
}; 