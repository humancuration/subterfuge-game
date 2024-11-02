#pragma once
#include "../Map/FoodWebSystem.hpp"
#include <string>
#include <vector>

class SpeciesIdentity {
public:
    enum class Category {
        MAMMAL,
        BIRD,
        REPTILE,
        AMPHIBIAN,
        FISH,
        INSECT,
        PLANT_BASED,  // For fungal or plant-derived sentients
        SYNTHETIC     // For AIs and artificial life
    };

    struct DietaryNature {
        enum class Type {
            CARNIVORE,
            HERBIVORE,
            OMNIVORE,
            PHOTOSYNTHETIC,  // For plant-based species
            SYNTHETIC_ENERGY, // For AIs/synthetics
            EVOLVED_VEGAN,   // Species that evolved past their original diet
            ENERGY_SYMBIOTE  // Species that developed mutual energy sharing
        };

        Type base_type;
        Type current_type;  // Can change through cultural evolution
        float ethical_awareness{0.0f};  // Understanding of dietary impact
        std::vector<std::string> traditional_prey;
        std::vector<std::string> cultural_taboos;
    };

    struct CulturalEvolution {
        float interspecies_empathy{0.0f};
        float dietary_adaptation{0.0f};
        float technological_compensation{0.0f}; // Using tech to overcome biological limits
        
        std::vector<std::string> evolved_practices;
        std::vector<std::string> abandoned_instincts;
    };

    struct SpeciesRelations {
        struct Relationship {
            float trust_level{0.0f};
            float historical_conflict{0.0f};
            float cultural_exchange{0.0f};
            bool were_traditional_prey{false};
            bool were_traditional_predator{false};
            std::vector<std::string> shared_values;
            std::vector<std::string> points_of_tension;
        };

        std::unordered_map<std::string, Relationship> relations;
    };

private:
    Category species_category;
    DietaryNature dietary_nature;
    CulturalEvolution cultural_evolution;
    SpeciesRelations relations;

    // Evolutionary history
    std::vector<std::string> ancestral_traits;
    std::vector<std::string> evolved_traits;
    float evolutionary_divergence{0.0f};

public:
    void update_species_relations(float delta_time) {
        for (auto& [species_name, relation] : relations.relations) {
            // Natural improvement in relations through cultural exchange
            if (relation.cultural_exchange > 0.5f) {
                relation.trust_level += delta_time * 0.1f * 
                    (cultural_evolution.interspecies_empathy * 0.5f);
            }

            // Historical predator-prey dynamics can be overcome
            if (relation.were_traditional_prey || relation.were_traditional_predator) {
                float healing_rate = cultural_evolution.interspecies_empathy * 
                    dietary_nature.ethical_awareness * delta_time;
                relation.historical_conflict = std::max(0.0f, 
                    relation.historical_conflict - healing_rate);
            }
        }
    }

    void evolve_dietary_practices(float delta_time) {
        // Species can evolve beyond their biological dietary nature
        if (dietary_nature.ethical_awareness > 0.8f && 
            cultural_evolution.technological_compensation > 0.7f) {
            
            if (dietary_nature.current_type != DietaryNature::Type::EVOLVED_VEGAN) {
                dietary_nature.current_type = DietaryNature::Type::EVOLVED_VEGAN;
                cultural_evolution.evolved_practices.push_back("Ethical_Nutrition");
                cultural_evolution.abandoned_instincts.push_back("Predatory_Drive");
            }
        }
    }

    void develop_cultural_exchange(const std::string& other_species, float interaction_quality) {
        auto& relation = relations.relations[other_species];
        
        // Positive interactions build trust and cultural exchange
        if (interaction_quality > 0) {
            relation.cultural_exchange += interaction_quality * 0.1f;
            relation.trust_level += interaction_quality * 0.05f;
            
            // Species can develop shared values
            if (relation.cultural_exchange > 0.8f) {
                relation.shared_values.push_back("Interspecies_Harmony");
            }
        }
    }

    bool can_form_alliance_with(const std::string& other_species) const {
        const auto& relation = relations.relations.at(other_species);
        
        // Historical predator-prey relationships can be overcome through 
        // strong cultural evolution and ethical awareness
        bool historical_conflicts_resolved = 
            !relation.were_traditional_prey || 
            (relation.historical_conflict < 0.2f && 
             cultural_evolution.interspecies_empathy > 0.8f);
        
        return relation.trust_level > 0.7f && 
               historical_conflicts_resolved &&
               !relation.shared_values.empty();
    }

    float calculate_dietary_tension(const SpeciesIdentity& other) const {
        float tension = 0.0f;
        
        // Base tension from predator-prey relationship
        if (dietary_nature.base_type == DietaryNature::Type::CARNIVORE &&
            other.dietary_nature.base_type == DietaryNature::Type::HERBIVORE) {
            tension += 0.5f;
        }
        
        // Reduced by ethical evolution
        tension *= (1.0f - dietary_nature.ethical_awareness);
        tension *= (1.0f - cultural_evolution.interspecies_empathy);
        
        // Cultural practices can eliminate tension
        if (dietary_nature.current_type == DietaryNature::Type::EVOLVED_VEGAN ||
            dietary_nature.current_type == DietaryNature::Type::ENERGY_SYMBIOTE) {
            tension *= 0.1f;
        }
        
        return tension;
    }
}; 