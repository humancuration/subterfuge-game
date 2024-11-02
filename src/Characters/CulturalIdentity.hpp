#pragma once
#include "../Map/CulturalEvolutionSystem.hpp"
#include <unordered_map>

class CulturalIdentity {
public:
    struct CulturalAffiliation {
        float adherence_level{0.0f};      // How strongly they follow this culture
        float integration_level{0.0f};     // How well they've integrated practices
        float innovation_tendency{0.0f};   // Likelihood to create new variations
        std::vector<std::string> personal_interpretations;
        bool is_childhood_culture{false};
    };

    struct ValueSystem {
        struct Value {
            float importance{0.0f};
            float flexibility{0.0f};       // Willingness to modify this value
            std::string origin_culture;
            bool is_core_belief{false};
        };
        
        std::unordered_map<std::string, Value> values;
        float internal_conflict{0.0f};     // Tracks conflicting values
    };

    struct CulturalExpression {
        enum class Type {
            FASHION,            // Clothing and appearance
            CUISINE,            // Food preferences and cooking
            LANGUAGE,           // Speech patterns and dialects
            RITUAL,             // Personal ceremonies/habits
            ARTISTIC,           // Creative expression
            SOCIAL_NORMS,       // Behavioral patterns
            PHILOSOPHY         // Personal beliefs
        };

        Type type;
        std::vector<std::string> source_cultures;
        float authenticity{1.0f};
        float personal_innovation{0.0f};
        bool is_fusion{false};
    };

private:
    std::unordered_map<std::string, CulturalAffiliation> cultural_affiliations;
    ValueSystem personal_values;
    std::vector<CulturalExpression> expressions;
    
    // Track cultural journey
    struct CulturalExperience {
        std::string culture;
        float impact_level;
        std::string context;
        bool was_positive;
    };
    std::vector<CulturalExperience> cultural_history;

public:
    void update_cultural_identity(float delta_time) {
        process_value_conflicts(delta_time);
        evolve_cultural_expressions(delta_time);
        integrate_new_influences(delta_time);
    }

    void expose_to_culture(const std::string& culture_name, float intensity) {
        auto& affiliation = cultural_affiliations[culture_name];
        
        // Initial exposure or deepening connection
        if (affiliation.adherence_level == 0.0f) {
            // First contact
            cultural_history.push_back({
                culture_name,
                intensity,
                "First Exposure",
                true
            });
        }

        // Integration process
        float integration_rate = calculate_integration_rate(culture_name, intensity);
        affiliation.integration_level += integration_rate;
        
        // Possible value adoption
        consider_adopting_values(culture_name, intensity);
    }

private:
    void process_value_conflicts(float delta_time) {
        float total_conflict = 0.0f;
        
        // Check for conflicting values
        for (const auto& [value1, data1] : personal_values.values) {
            for (const auto& [value2, data2] : personal_values.values) {
                if (value1 != value2 && are_values_conflicting(value1, value2)) {
                    // Calculate conflict intensity
                    float conflict = data1.importance * data2.importance;
                    
                    // More flexible values reduce conflict
                    conflict *= (2.0f - data1.flexibility - data2.flexibility);
                    
                    total_conflict += conflict;
                    
                    // Possibly resolve conflict through value evolution
                    resolve_value_conflict(value1, value2, conflict);
                }
            }
        }
        
        personal_values.internal_conflict = total_conflict;
    }

    void evolve_cultural_expressions(float delta_time) {
        for (auto& expression : expressions) {
            // Personal innovation in cultural expression
            if (should_innovate(expression)) {
                float innovation_amount = calculate_innovation_amount(expression);
                expression.personal_innovation += innovation_amount;
                
                // Might create fusion expression
                if (expression.personal_innovation > 0.7f && 
                    expression.source_cultures.size() > 1) {
                    expression.is_fusion = true;
                }
            }
            
            // Update authenticity based on understanding and practice
            update_expression_authenticity(expression, delta_time);
        }
    }

    void integrate_new_influences(float delta_time) {
        for (auto& [culture_name, affiliation] : cultural_affiliations) {
            if (affiliation.integration_level > 0.0f) {
                // Deeper understanding develops over time
                float understanding_rate = calculate_understanding_rate(affiliation);
                affiliation.integration_level += understanding_rate * delta_time;
                
                // Might develop personal interpretations
                if (should_develop_interpretation(affiliation)) {
                    create_personal_interpretation(culture_name, affiliation);
                }
            }
        }
    }

    void create_personal_interpretation(const std::string& culture_name, 
                                     CulturalAffiliation& affiliation) {
        // Combine elements from different cultural backgrounds
        std::string interpretation = generate_cultural_fusion(
            culture_name,
            affiliation.integration_level,
            get_strongest_other_culture(culture_name)
        );
        
        affiliation.personal_interpretations.push_back(interpretation);
    }

    std::string generate_cultural_fusion(const std::string& primary_culture,
                                       float integration_level,
                                       const std::string& secondary_culture) {
        // Create unique cultural expressions based on personal experience
        // This could be anything from fusion cuisine to hybrid philosophical views
        return "Fusion_" + primary_culture + "_" + secondary_culture;
    }

    std::string get_strongest_other_culture(const std::string& exclude_culture) {
        std::string strongest;
        float max_adherence = 0.0f;
        
        for (const auto& [culture, affiliation] : cultural_affiliations) {
            if (culture != exclude_culture && 
                affiliation.adherence_level > max_adherence) {
                max_adherence = affiliation.adherence_level;
                strongest = culture;
            }
        }
        
        return strongest;
    }
}; 