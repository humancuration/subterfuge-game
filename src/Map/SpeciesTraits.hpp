#pragma once
#include <string>
#include <vector>
#include <functional>

class SpeciesTraits {
public:
    // Life History Traits
    struct LifeHistoryStrategy {
        float maturity_age;        // Time to reach breeding age
        float lifespan;            // Maximum lifespan
        float breeding_frequency;   // How often they can reproduce
        float offspring_count;      // Number of offspring per breeding
        float parental_care;       // Investment in offspring
        float energy_efficiency;   // How efficiently they use resources
    };

    // Social Behavior Traits
    struct SocialBehavior {
        enum class GroupStructure {
            SOLITARY,
            PAIRS,
            SMALL_GROUPS,
            LARGE_HERDS,
            HIERARCHICAL
        };

        GroupStructure structure;
        float cooperation_level;    // How much they work together
        float territory_size;       // Area they defend
        float aggression_level;     // Tendency for conflict
        bool migratory;            // Whether they migrate seasonally
    };

    // Physical Characteristics
    struct PhysicalTraits {
        float body_size;           // Relative size (0-1)
        float speed;               // Movement capability
        float stamina;             // Endurance
        float camouflage;          // Ability to hide
        float sensory_acuity;      // Perception abilities
        std::vector<std::string> specialized_organs; // Special adaptations
    };

    // Dietary Specialization
    struct DietaryTraits {
        enum class FeedingStrategy {
            HERBIVORE,
            CARNIVORE,
            OMNIVORE,
            FILTER_FEEDER,
            DETRITIVORE,
            SYMBIOTIC
        };

        FeedingStrategy strategy;
        float food_size_preference; // Preferred prey/plant size
        float digestion_efficiency;
        std::vector<std::string> preferred_foods;
        bool opportunistic;         // Can eat outside preferences
    };

    // Environmental Adaptations
    struct EnvironmentalAdaptations {
        struct TemperatureRange {
            float optimal;
            float min_survivable;
            float max_survivable;
        } temperature;

        float depth_preference;     // Preferred water depth
        float pressure_tolerance;   // Deep water adaptation
        float salinity_tolerance;   // Salt water adaptation
        float pollution_resistance; // Toxin resistance
        float oxygen_efficiency;    // Low oxygen adaptation
    };

    // Competitive Advantages
    struct CompetitiveTraits {
        float resource_efficiency;  // Resource use efficiency
        float stress_tolerance;     // Environmental stress handling
        float immune_strength;      // Disease resistance
        float adaptation_rate;      // How quickly they can adapt
        float genetic_diversity;    // Population genetic variation
    };

    // Calculate fitness based on all traits and environment
    float calculate_fitness(const EnvironmentalConditions& environment) {
        float fitness = 1.0f;

        // Life History Impact
        fitness *= calculate_life_history_fitness(environment);
        
        // Social Behavior Impact
        fitness *= calculate_social_fitness(environment);
        
        // Physical Traits Impact
        fitness *= calculate_physical_fitness(environment);
        
        // Dietary Success
        fitness *= calculate_dietary_fitness(environment);
        
        // Environmental Match
        fitness *= calculate_environmental_fitness(environment);
        
        // Competitive Success
        fitness *= calculate_competitive_fitness(environment);

        return fitness;
    }

private:
    float calculate_life_history_fitness(const EnvironmentalConditions& env) {
        float fitness = 1.0f;
        
        // R vs K selection pressure
        if (env.resource_abundance > 0.7f) {
            // Rich environment favors K-selection
            fitness *= life_history.parental_care * life_history.energy_efficiency;
        } else {
            // Poor environment favors r-selection
            fitness *= life_history.breeding_frequency * life_history.offspring_count;
        }

        return fitness;
    }

    float calculate_social_fitness(const EnvironmentalConditions& env) {
        float fitness = 1.0f;
        
        // Group living advantages/disadvantages
        switch (social_behavior.structure) {
            case SocialBehavior::GroupStructure::LARGE_HERDS:
                // Better predator detection but more competition
                fitness *= (env.predation_pressure * 1.5f) / 
                          (env.resource_competition * 0.8f);
                break;
            case SocialBehavior::GroupStructure::SOLITARY:
                // Less competition but more vulnerable
                fitness *= (env.resource_abundance * 1.2f) / 
                          (env.predation_pressure * 0.7f);
                break;
        }

        return fitness;
    }

    float calculate_physical_fitness(const EnvironmentalConditions& env) {
        float fitness = 1.0f;
        
        // Size-based advantages/disadvantages
        if (physical.body_size > 0.7f) {
            // Large size: better competition, worse food scarcity
            fitness *= (1.0f + env.resource_abundance) / 
                      (2.0f - env.competition_intensity);
        } else {
            // Small size: better hiding, worse competition
            fitness *= (2.0f - env.predation_pressure) / 
                      (1.0f + env.competition_intensity);
        }

        return fitness;
    }

    float calculate_dietary_fitness(const EnvironmentalConditions& env) {
        float fitness = 1.0f;
        
        // Specialist vs Generalist trade-offs
        if (dietary.opportunistic) {
            // Generalists: more stable but less efficient
            fitness *= 0.8f + (env.food_diversity * 0.4f);
        } else {
            // Specialists: more efficient but vulnerable
            fitness *= dietary.digestion_efficiency * 
                      env.preferred_food_availability;
        }

        return fitness;
    }
}; 