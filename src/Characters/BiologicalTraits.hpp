#pragma once
#include "../Map/SpeciesTraits.hpp"
#include "../Map/FoodWebSystem.hpp"
#include <string>
#include <vector>

class BiologicalTraits {
public:
    struct DietaryPreference {
        enum class Type {
            CARNIVORE,
            HERBIVORE,
            OMNIVORE,
            VEGAN,
            LOCAVORE,     // Prefers local food
            SUSTAINABLE   // Focuses on sustainable food sources
        };

        Type preference;
        float environmental_impact;
        float health_impact;
        float local_economy_impact;
    };

    struct EnvironmentalAwareness {
        float conservation_tendency{0.5f};  // How likely to choose eco-friendly options
        float resource_efficiency{0.5f};    // How efficiently they use resources
        float pollution_sensitivity{0.5f};  // How much they care about pollution
        float wildlife_affinity{0.5f};     // Connection to local wildlife
        
        std::vector<std::string> known_species;
        std::vector<std::string> protected_habitats;
    };

    struct CulturalPractices {
        bool practices_composting{false};
        bool grows_own_food{false};
        bool participates_in_conservation{false};
        float community_influence{0.5f};
        
        std::vector<std::string> sustainable_traditions;
        std::vector<std::string> teaching_methods;
    };

private:
    DietaryPreference diet;
    EnvironmentalAwareness awareness;
    CulturalPractices practices;
    
    // Impact tracking
    float cumulative_environmental_impact{0.0f};
    float positive_ecosystem_contribution{0.0f};
    
    std::vector<std::string> learned_environmental_lessons;

public:
    void update_environmental_impact(const FoodWebSystem& food_web, float delta_time) {
        // Calculate impact based on dietary choices
        float impact = calculate_dietary_impact(food_web);
        
        // Modify based on environmental awareness
        impact *= (2.0f - awareness.conservation_tendency);
        
        // Apply cultural practice benefits
        if (practices.grows_own_food) {
            impact *= 0.7f;  // 30% reduction
        }
        if (practices.practices_composting) {
            impact *= 0.9f;  // 10% reduction
        }
        
        cumulative_environmental_impact += impact * delta_time;
        
        // Calculate positive contributions
        if (practices.participates_in_conservation) {
            positive_ecosystem_contribution += 
                awareness.conservation_tendency * delta_time;
        }
    }

    void learn_from_environment(const SpeciesTraits& local_species) {
        // Learn about local wildlife
        if (awareness.wildlife_affinity > 0.7f) {
            for (const auto& trait : local_species.get_observable_traits()) {
                if (std::find(learned_environmental_lessons.begin(),
                            learned_environmental_lessons.end(),
                            trait) == learned_environmental_lessons.end()) {
                    learned_environmental_lessons.push_back(trait);
                    awareness.wildlife_affinity += 0.1f;
                }
            }
        }
    }

    void spread_environmental_knowledge(std::vector<Character*>& nearby_characters) {
        if (practices.community_influence > 0.5f) {
            for (auto* character : nearby_characters) {
                // Share sustainable practices
                for (const auto& practice : practices.sustainable_traditions) {
                    character->consider_adopting_practice(practice, 
                        practices.community_influence);
                }
                
                // Share species knowledge
                for (const auto& species : awareness.known_species) {
                    character->learn_about_species(species, 
                        awareness.wildlife_affinity);
                }
            }
        }
    }

    void react_to_environmental_change(const EnvironmentalEvent& event) {
        switch (event.type) {
            case EnvironmentalEvent::Type::SPECIES_DECLINE:
                awareness.conservation_tendency += 0.1f;
                if (!practices.participates_in_conservation && 
                    awareness.conservation_tendency > 0.8f) {
                    practices.participates_in_conservation = true;
                }
                break;
                
            case EnvironmentalEvent::Type::HABITAT_DESTRUCTION:
                awareness.pollution_sensitivity += 0.15f;
                if (!practices.grows_own_food && 
                    awareness.resource_efficiency > 0.7f) {
                    practices.grows_own_food = true;
                }
                break;
        }
    }

    float calculate_dietary_impact(const FoodWebSystem& food_web) {
        float impact = 0.0f;
        
        switch (diet.preference) {
            case DietaryPreference::Type::CARNIVORE:
                impact = 1.0f;  // Baseline impact
                break;
            case DietaryPreference::Type::HERBIVORE:
                impact = 0.5f;
                break;
            case DietaryPreference::Type::VEGAN:
                impact = 0.3f;
                break;
            case DietaryPreference::Type::LOCAVORE:
                // Calculate based on local food availability
                impact = food_web.calculate_local_food_impact(
                    awareness.resource_efficiency);
                break;
        }
        
        return impact;
    }
}; 