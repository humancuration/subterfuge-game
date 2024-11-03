#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "../Core/JobSystem.hpp"
#include <unordered_map>

class ClimateSystem : public godot::Node3D {
    GDCLASS(ClimateSystem, Node3D)

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
        
        std::function<void(class Waypoint*, float)> hazard_effect;
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
    static constexpr float NEOFUEL_ACCIDENT_BASE_CHANCE = 0.001f;
    static constexpr float NEOFUEL_MUTATION_CHANCE = 0.005f;
    static constexpr float WATER_CONTAMINATION_RADIUS = 100.0f;
    static constexpr float RADIATION_SPREAD_RATE = 0.1f;

protected:
    static void _bind_methods();

public:
    ClimateSystem();
    
    void update_climate(float delta);
    void simulate_resource_extraction(float delta);
    
    // Getters
    float get_temperature(const godot::Vector2& position) const;
    float get_humidity(const godot::Vector2& position) const;
    float get_air_quality(const godot::Vector2& position) const;
    float get_radiation_level(const godot::Vector2& position) const;
    
    // Hazard management
    void add_hazard(const EnvironmentalHazard& hazard);
    void remove_hazard(const godot::Vector2& position, float radius);
    Array get_active_hazards() const;
    
    // Resource management
    void add_extraction_site(const godot::Vector2& position, float rate);
    void remove_extraction_site(const godot::Vector2& position);
    float get_resource_concentration(const godot::Vector2& position) const;

    // Original functionality restored
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

private:
    void update_climate_batch_simd(size_t start_idx, size_t count, float delta_time) {
        for (size_t i = 0; i < count; i += 8) {
            __m256 temps = _mm256_load_ps(&temperatures[start_idx + i]);
            __m256 pollution = _mm256_load_ps(&pollution_levels[start_idx + i]);
            __m256 radiation = _mm256_load_ps(&radiation_values[start_idx + i]);
            
            // Process climate changes using SIMD
            process_climate_changes_simd(temps, pollution, radiation, delta_time);
            
            _mm256_store_ps(&temperatures[start_idx + i], temps);
            _mm256_store_ps(&pollution_levels[start_idx + i], pollution);
            _mm256_store_ps(&radiation_values[start_idx + i], radiation);
        }
    }

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
        
        site.local_hazards.push_back(crystal)

    void process_hazard_effects() {
        for (auto& hazard : global_hazards) {
            if (!hazard.is_permanent) {
                hazard.intensity *= (1.0f - hazard.decay_rate);
            }
            
            // Apply hazard effects to nearby waypoints
            auto affected_waypoints = get_waypoints_in_radius(hazard.position, hazard.radius);
            for (auto* waypoint : affected_waypoints) {
                hazard.hazard_effect(waypoint, hazard.intensity);
            }
        }
    }

    void contaminate_water_supply(const godot::Vector2& position, float radius) {
        // Implement water table contamination that spreads over time
        // This affects multiple climate cells and persists
        for (auto& row : climate_grid) {
            for (auto& cell : row) {
                float distance = position.distance_to(godot::Vector2(cell.position));
                if (distance < radius) {
                    float contamination = 1.0f - (distance / radius);
                    cell.water_pollution += contamination;
                    cell.ground_water_quality *= (1.0f - contamination * 0.5f);
                }
            }
        }
    }
}; 