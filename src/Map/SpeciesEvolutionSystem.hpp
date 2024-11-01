#pragma once
#include "MarineEcosystemSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <random>

class SpeciesEvolutionSystem {
private:
    struct Gene {
        std::string trait_id;
        float expression_level;
        float mutation_chance;
        bool is_dominant;
    };

    struct Trait {
        enum class Category {
            TEMPERATURE_TOLERANCE,
            DIET_TYPE,
            HABITAT_PREFERENCE,
            REPRODUCTION_STRATEGY,
            DEFENSE_MECHANISM,
            SOCIAL_BEHAVIOR,
            MIGRATION_PATTERN,
            PHYSICAL_ATTRIBUTES
        };

        Category category;
        float fitness_contribution;
        std::vector<std::string> incompatible_traits;
        std::function<void(Species*)> expression_effect;
    };

    struct Species {
        std::string id;
        std::string scientific_name;
        std::string common_name;
        
        // Conservation status
        enum class Status {
            UNDISCOVERED,
            NEWLY_OBSERVED,
            LEAST_CONCERN,
            VULNERABLE,
            ENDANGERED,
            CRITICALLY_ENDANGERED,
            EXTINCT_IN_WILD,
            EXTINCT
        } conservation_status;

        // Population data
        struct PopulationData {
            size_t total_population{0};
            size_t breeding_population{0};
            float growth_rate{0.0f};
            float death_rate{0.0f};
            std::unordered_map<std::string, size_t> regional_populations;
        } population;

        // Environmental tolerances
        struct EnvironmentalTolerance {
            float min_temperature;
            float max_temperature;
            float min_depth;
            float max_depth;
            float salinity_preference;
            float ph_tolerance;
            float pollution_resistance;
        } tolerances;

        // Genetic information
        std::vector<Gene> genome;
        float genetic_diversity;
        float mutation_rate;
        
        // Ecological relationships
        std::vector<std::string> prey_species;
        std::vector<std::string> predator_species;
        std::vector<std::string> symbiotic_relationships;
        
        // Behavioral traits
        bool is_migratory;
        bool is_social;
        bool is_territorial;
        float intelligence_level;
    };

    // SIMD-aligned evolution data
    alignas(32) std::vector<float> fitness_scores;
    alignas(32) std::vector<float> mutation_rates;
    alignas(32) std::vector<float> population_growths;
    
    std::vector<Species> known_species;
    std::vector<Species> undiscovered_species;
    std::unordered_map<std::string, Trait> trait_database;
    
    std::mt19937 rng{std::random_device{}()};

public:
    void update_species_evolution(float delta_time, const OceanSystem& ocean) {
        process_speciation_events();
        update_populations_simd(delta_time);
        check_for_new_species();
        update_conservation_status();
    }

private:
    void process_speciation_events() {
        for (auto& species : known_species) {
            if (should_attempt_speciation(species)) {
                attempt_speciation(species);
            }
        }
    }

    bool should_attempt_speciation(const Species& species) {
        // Factors that increase speciation chance:
        // - High genetic diversity
        // - Large, stable population
        // - Geographic isolation
        // - Environmental pressure
        float speciation_chance = calculate_speciation_chance(species);
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        return dis(rng) < speciation_chance;
    }

    void attempt_speciation(const Species& parent_species) {
        Species new_species = parent_species;  // Inherit base traits
        
        // Modify genome with mutations
        mutate_genome(new_species.genome);
        
        // Generate new species names
        generate_species_names(new_species);
        
        // Start with small population from parent species
        new_species.population.total_population = 
            parent_species.population.total_population * 0.01f;
        
        // Initially undiscovered
        new_species.conservation_status = Species::Status::UNDISCOVERED;
        
        undiscovered_species.push_back(new_species);
    }

    void update_populations_simd(float delta_time) {
        const size_t species_count = known_species.size();
        
        for (size_t i = 0; i < species_count; i += 8) {
            __m256 populations = _mm256_load_ps(&population_growths[i]);
            __m256 growth_rates = calculate_growth_rates_simd(i);
            __m256 death_rates = calculate_death_rates_simd(i);
            
            // Update populations
            populations = _mm256_add_ps(
                populations,
                _mm256_mul_ps(
                    _mm256_sub_ps(growth_rates, death_rates),
                    _mm256_set1_ps(delta_time)
                )
            );
            
            _mm256_store_ps(&population_growths[i], populations);
        }
    }

    void check_for_new_species() {
        // Simulate scientific discovery process
        for (auto it = undiscovered_species.begin(); it != undiscovered_species.end();) {
            if (should_discover_species(*it)) {
                it->conservation_status = Species::Status::NEWLY_OBSERVED;
                known_species.push_back(*it);
                it = undiscovered_species.erase(it);
            } else {
                ++it;
            }
        }
    }

    bool should_discover_species(const Species& species) {
        // Factors affecting discovery:
        // - Population size
        // - Habitat accessibility
        // - Distinctiveness from known species
        // - Research activity in the area
        float discovery_chance = calculate_discovery_chance(species);
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        return dis(rng) < discovery_chance;
    }

    void update_conservation_status() {
        for (auto& species : known_species) {
            auto new_status = calculate_conservation_status(species);
            if (new_status != species.conservation_status) {
                species.conservation_status = new_status;
                notify_conservation_status_change(species);
            }
        }
    }

    Species::Status calculate_conservation_status(const Species& species) {
        const size_t population = species.population.total_population;
        const float growth_rate = species.population.growth_rate;
        
        if (population == 0) return Species::Status::EXTINCT;
        if (population < 50) return Species::Status::CRITICALLY_ENDANGERED;
        if (population < 250) return Species::Status::ENDANGERED;
        if (population < 1000) return Species::Status::VULNERABLE;
        if (growth_rate > 0.01f && population > 10000) return Species::Status::LEAST_CONCERN;
        
        return Species::Status::VULNERABLE;
    }
}; 