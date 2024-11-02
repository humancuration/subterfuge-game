#pragma once
#include "CulturalIdentity.hpp"
#include <unordered_map>

class ConflictResolutionSystem {
public:
    struct ValueConflict {
        enum class Type {
            MORAL,              // Ethical principles clash
            CULTURAL,           // Different cultural practices
            RELIGIOUS,          // Spiritual beliefs
            IDEOLOGICAL,        // Political/social views
            TRADITIONAL,        // Old vs new ways
            GENERATIONAL,       // Age-based differences
            ENVIRONMENTAL,      // Resource use vs preservation
            ECONOMIC           // Material vs spiritual values
        };

        Type type;
        float intensity{1.0f};
        float resolution_progress{0.0f};
        bool is_fundamental{false};  // Some conflicts can't be fully resolved
    };

    struct ResolutionStrategy {
        enum class Approach {
            COMPROMISE,         // Find middle ground
            SYNTHESIS,          // Create new hybrid solution
            ADAPTATION,         // One side adapts to other
            TRANSFORMATION,     // Both sides evolve to new understanding
            COMPARTMENTALIZATION, // Keep conflicting values separate
            INTEGRATION,        // Blend both perspectives
            INNOVATION,         // Create entirely new solution
            PEACEFUL_COEXISTENCE // Accept differences
        };

        Approach approach;
        float success_chance{0.5f};
        float stability{1.0f};
        std::vector<std::string> required_traits;
    };

private:
    struct ResolutionProgress {
        float understanding_level{0.0f};
        float empathy_growth{0.0f};
        float compromise_willingness{0.0f};
        std::vector<std::string> learned_lessons;
    };

    struct ConflictMemory {
        std::string conflict_type;
        float resolution_success;
        ResolutionStrategy::Approach successful_approach;
        std::vector<std::string> key_insights;
    };

public:
    void resolve_value_conflict(CulturalIdentity& identity, 
                              const std::string& value1,
                              const std::string& value2) {
        ValueConflict conflict = analyze_conflict(value1, value2);
        
        // Try different resolution strategies
        for (const auto& strategy : get_viable_strategies(conflict)) {
            if (attempt_resolution(identity, conflict, strategy)) {
                record_successful_resolution(identity, conflict, strategy);
                apply_resolution_effects(identity, conflict, strategy);
                break;
            }
        }
    }

private:
    ValueConflict analyze_conflict(const std::string& value1, 
                                 const std::string& value2) {
        ValueConflict conflict;
        
        // Determine conflict type and intensity
        if (are_values_fundamentally_opposed(value1, value2)) {
            conflict.type = ValueConflict::Type::MORAL;
            conflict.is_fundamental = true;
            conflict.intensity = 1.0f;
        } else if (are_culturally_different(value1, value2)) {
            conflict.type = ValueConflict::Type::CULTURAL;
            conflict.intensity = 0.7f;
        }
        // ... other conflict type analyses
        
        return conflict;
    }

    bool attempt_resolution(CulturalIdentity& identity,
                          const ValueConflict& conflict,
                          const ResolutionStrategy& strategy) {
        float base_chance = strategy.success_chance;
        
        // Modify success chance based on character traits
        base_chance *= calculate_trait_modifier(identity, strategy);
        
        // Cultural flexibility increases success chance
        if (identity.has_trait("Cultural_Flexibility")) {
            base_chance *= 1.2f;
        }
        
        // Previous experience helps
        if (has_resolved_similar_conflict(identity, conflict)) {
            base_chance *= 1.3f;
        }
        
        // Attempt resolution
        return random_float() < base_chance;
    }

    void apply_resolution_effects(CulturalIdentity& identity,
                                const ValueConflict& conflict,
                                const ResolutionStrategy& strategy) {
        switch (strategy.approach) {
            case ResolutionStrategy::Approach::SYNTHESIS:
                create_syncretic_practice(identity, conflict);
                break;
            case ResolutionStrategy::Approach::TRANSFORMATION:
                evolve_cultural_values(identity, conflict);
                break;
            case ResolutionStrategy::Approach::INTEGRATION:
                integrate_different_perspectives(identity, conflict);
                break;
        }
        
        // Personal growth from conflict resolution
        identity.increase_trait("Wisdom", 0.1f);
        identity.increase_trait("Cultural_Understanding", 0.15f);
        
        // Record insights
        record_cultural_learning(identity, conflict, strategy);
    }

    void create_syncretic_practice(CulturalIdentity& identity,
                                 const ValueConflict& conflict) {
        // Combine elements from conflicting values
        std::string new_practice = generate_syncretic_practice(
            conflict.type,
            identity.get_cultural_background()
        );
        
        // Add new hybrid practice
        identity.add_cultural_practice(new_practice);
        
        // Increase cultural synthesis ability
        identity.increase_trait("Cultural_Synthesis", 0.2f);
    }

    void evolve_cultural_values(CulturalIdentity& identity,
                              const ValueConflict& conflict) {
        // Transform conflicting values into new understanding
        std::string evolved_value = generate_evolved_value(
            conflict.type,
            identity.get_current_values()
        );
        
        // Replace old values with evolved understanding
        identity.replace_value(evolved_value);
        
        // Increase wisdom from transformation
        identity.increase_trait("Wisdom", 0.3f);
    }

    void record_cultural_learning(CulturalIdentity& identity,
                                const ValueConflict& conflict,
                                const ResolutionStrategy& strategy) {
        ConflictMemory memory{
            conflict_type_to_string(conflict.type),
            1.0f,
            strategy.approach,
            generate_insights(conflict, strategy)
        };
        
        identity.add_conflict_memory(memory);
    }

    std::vector<std::string> generate_insights(const ValueConflict& conflict,
                                             const ResolutionStrategy& strategy) {
        std::vector<std::string> insights;
        
        // Generate specific insights based on conflict type and resolution
        switch (conflict.type) {
            case ValueConflict::Type::CULTURAL:
                insights.push_back("Different cultures can enrich each other");
                insights.push_back("Traditions can evolve while keeping their essence");
                break;
            case ValueConflict::Type::ENVIRONMENTAL:
                insights.push_back("Balance between progress and preservation");
                insights.push_back("Innovation can protect tradition");
                break;
        }
        
        return insights;
    }
}; 