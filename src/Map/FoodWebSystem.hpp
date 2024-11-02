#pragma once
#include "SpeciesTraits.hpp"
#include "../Core/JobSystem.hpp"
#include <unordered_map>

class FoodWebSystem {
private:
    struct EcologicalRelationship {
        enum class Type {
            PREDATOR_PREY,      // Traditional hunting
            GRAZING,            // Sustainable plant consumption
            SCAVENGING,         // Consuming already dead organisms
            PARASITIC,          // One benefits, other harmed
            COMMENSAL,          // One benefits, other unaffected
            MUTUALISTIC,        // Both benefit (e.g., pollinators)
            COMPETITIVE,        // Compete for same resources
            FACILITATIVE        // One species makes environment better for another
        };

        Type type;
        float intensity;        // How strong the relationship is
        float sustainability;   // How sustainable the relationship is
        bool is_obligate;      // Whether relationship is necessary for survival
    };

    struct BiomassFlow {
        float energy_transfer_efficiency;
        float nutrient_recycling_rate;
        float waste_production;
        float decomposition_rate;
    };

    struct PopulationDynamics {
        float carrying_capacity;
        float growth_rate;
        float density_dependence;
        float minimum_viable_population;
        
        // Factors affecting population
        float food_availability;
        float habitat_quality;
        float predation_pressure;
        float disease_resistance;
    };

    struct BehavioralAdaptation {
        // Hunting/Foraging strategies
        float energy_expenditure;
        float success_rate;
        float risk_assessment;
        
        // Social behaviors
        bool forms_groups;
        float cooperation_level;
        float territorial_behavior;
        
        // Resource management
        float food_caching;
        float resource_sharing;
        float seasonal_adaptation;
    };

    struct EnvironmentalImpact {
        float soil_impact;
        float water_usage;
        float carbon_footprint;
        float habitat_modification;
        float nutrient_cycling_contribution;
    };

public:
    void update_food_web(float delta_time) {
        update_population_dynamics(delta_time);
        process_biomass_flows(delta_time);
        adjust_behavioral_patterns(delta_time);
        calculate_environmental_impacts(delta_time);
    }

private:
    void update_population_dynamics(float delta_time) {
        for (auto& [species, dynamics] : population_dynamics) {
            // Calculate sustainable population level
            float sustainable_pop = calculate_sustainable_population(
                dynamics.carrying_capacity,
                dynamics.food_availability,
                dynamics.predation_pressure
            );

            // Natural population adjustment
            if (dynamics.current_population > sustainable_pop) {
                // Population naturally decreases when above sustainable level
                dynamics.current_population *= (1.0f - 
                    (dynamics.density_dependence * delta_time));
            } else if (dynamics.current_population > dynamics.minimum_viable_population) {
                // Population grows when conditions are good
                dynamics.current_population *= (1.0f + 
                    (dynamics.growth_rate * delta_time));
            }
        }
    }

    void process_biomass_flows(float delta_time) {
        for (auto& [predator, prey] : ecological_relationships) {
            if (predator.relationship_type == EcologicalRelationship::Type::PREDATOR_PREY) {
                // Calculate sustainable predation rate
                float sustainable_rate = calculate_sustainable_predation(
                    predator.population,
                    prey.population,
                    predator.energy_needs,
                    prey.reproduction_rate
                );

                // Adjust predation behavior based on prey population
                if (prey.population < prey.minimum_viable_population) {
                    // Predators naturally reduce hunting of scarce prey
                    predator.hunting_intensity *= 0.5f;
                    // Look for alternative food sources
                    find_alternative_prey(predator);
                }
            }
        }
    }

    void adjust_behavioral_patterns(float delta_time) {
        for (auto& [species, behavior] : behavioral_adaptations) {
            // Adjust group formation based on conditions
            if (behavior.forms_groups) {
                float optimal_group_size = calculate_optimal_group_size(
                    species.predation_pressure,
                    species.food_availability,
                    behavior.cooperation_level
                );
                
                behavior.group_size = std::lerp(
                    behavior.group_size,
                    optimal_group_size,
                    delta_time
                );
            }

            // Adjust territorial behavior
            behavior.territorial_intensity = calculate_territorial_need(
                species.population_density,
                species.resource_availability,
                species.breeding_season_active
            );
        }
    }

    void calculate_environmental_impacts(float delta_time) {
        for (auto& [species, impact] : environmental_impacts) {
            // Calculate sustainable resource usage
            float sustainable_usage = calculate_sustainable_resource_usage(
                species.population,
                species.consumption_rate,
                environment.resource_regeneration_rate
            );

            // Natural adjustment of resource usage
            if (impact.resource_usage > sustainable_usage) {
                // Species naturally reduces resource usage when scarce
                impact.resource_usage *= (1.0f - (0.1f * delta_time));
            }

            // Update environmental contribution
            impact.nutrient_cycling_contribution = calculate_nutrient_contribution(
                species.population,
                species.waste_production,
                species.decomposition_contribution
            );
        }
    }

    float calculate_sustainable_predation(float predator_pop, float prey_pop, 
                                        float energy_needs, float prey_reproduction) {
        // Complex calculation considering multiple factors
        float prey_sustainability = prey_pop * prey_reproduction;
        float predator_needs = predator_pop * energy_needs;
        
        // Return a rate that maintains prey population
        return std::min(prey_sustainability * 0.5f, predator_needs);
    }

    float calculate_optimal_group_size(float predation_pressure, 
                                     float food_availability,
                                     float cooperation_level) {
        // Larger groups when predation is high and food is plentiful
        float base_size = predation_pressure * 10.0f;
        float food_factor = std::min(food_availability, 1.0f);
        float coop_bonus = cooperation_level * 2.0f;
        
        return base_size * food_factor * (1.0f + coop_bonus);
    }
}; 