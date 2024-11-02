#pragma once
#include "../Actions/UniversalActionPool.hpp"
#include <unordered_set>

class EmergentTechnologySystem {
public:
    struct Discovery {
        std::string id;
        std::string name;
        
        enum class Category {
            TOOL_MAKING,
            AGRICULTURE,
            ENERGY_USE,
            COMMUNICATION,
            COMPUTATION,
            TRANSPORTATION,
            MEDICINE,
            ARCHITECTURE,
            SOCIAL_ORGANIZATION,
            ENVIRONMENTAL_CONTROL
        } category;

        // How this technology emerged
        struct Origin {
            std::vector<std::string> prerequisite_actions;
            std::vector<std::string> required_knowledge;
            std::vector<std::string> environmental_conditions;
            float complexity_threshold{0.0f};
            bool requires_cooperation{false};
        } origin;

        // What this enables
        struct Capabilities {
            std::vector<std::string> new_actions;
            std::vector<std::string> enhanced_abilities;
            float complexity_increase{0.0f};
            bool enables_teaching{false};
        } capabilities;
    };

private:
    // Track all possible and discovered technologies
    std::unordered_map<std::string, Discovery> potential_discoveries;
    std::unordered_map<std::string, std::unordered_set<std::string>> 
        species_discoveries;  // species_id -> discoveries

    // Track combinations that led to discoveries
    struct EmergentPattern {
        std::vector<std::string> contributing_actions;
        std::vector<std::string> environmental_factors;
        std::string resulting_discovery;
        float success_rate{0.0f};
    };
    std::vector<EmergentPattern> successful_patterns;

public:
    void initialize_potential_discoveries() {
        // Basic tool use might emerge from combining simple actions
        add_potential_discovery({
            "basic_tool_use",
            "Basic Tool Use",
            Discovery::Category::TOOL_MAKING,
            {
                {"manipulate_object", "observe_effect"},  // prerequisite actions
                {"material_properties"},                  // required knowledge
                {"tool_materials_present"},              // environmental conditions
                0.3f,                                    // complexity threshold
                false                                    // doesn't require cooperation
            },
            {
                {"use_simple_tool", "create_simple_tool"},  // new actions
                {"object_manipulation", "tool_knowledge"},   // enhanced abilities
                0.1f,                                       // complexity increase
                true                                        // can be taught
            }
        });

        // Agriculture might emerge from plant observation and tool use
        add_potential_discovery({
            "basic_agriculture",
            "Basic Agriculture",
            Discovery::Category::AGRICULTURE,
            {
                {"tend_plants", "use_simple_tool", "observe_growth"},
                {"plant_lifecycle", "soil_properties"},
                {"fertile_soil", "regular_water"},
                0.4f,
                true  // requires cooperation
            },
            {
                {"plant_crops", "create_garden", "store_food"},
                {"plant_knowledge", "soil_management"},
                0.2f,
                true
            }
        });
    }

    bool check_for_discovery(const std::string& species_id,
                           const std::vector<std::string>& current_actions,
                           const EnvironmentalState& env_state) {
        for (const auto& [tech_id, discovery] : potential_discoveries) {
            // Skip if already discovered
            if (species_discoveries[species_id].contains(tech_id)) {
                continue;
            }

            if (can_make_discovery(species_id, discovery, current_actions, env_state)) {
                float discovery_chance = calculate_discovery_chance(
                    species_id, discovery, current_actions
                );

                if (random_float() < discovery_chance) {
                    record_discovery(species_id, tech_id, current_actions, env_state);
                    return true;
                }
            }
        }
        return false;
    }

private:
    bool can_make_discovery(const std::string& species_id,
                          const Discovery& discovery,
                          const std::vector<std::string>& current_actions,
                          const EnvironmentalState& env_state) {
        // Check prerequisites
        bool has_required_actions = std::all_of(
            discovery.origin.prerequisite_actions.begin(),
            discovery.origin.prerequisite_actions.end(),
            [&](const std::string& action) {
                return std::find(current_actions.begin(), 
                               current_actions.end(), 
                               action) != current_actions.end();
            }
        );

        // Check environmental conditions
        bool env_conditions_met = check_environmental_conditions(
            discovery.origin.environmental_conditions,
            env_state
        );

        // Check species capabilities
        bool meets_complexity = get_species_complexity(species_id) >= 
                              discovery.origin.complexity_threshold;

        return has_required_actions && env_conditions_met && meets_complexity;
    }

    void record_discovery(const std::string& species_id,
                         const std::string& tech_id,
                         const std::vector<std::string>& contributing_actions,
                         const EnvironmentalState& env_state) {
        // Record the discovery for this species
        species_discoveries[species_id].insert(tech_id);

        // Record the pattern that led to discovery
        successful_patterns.push_back({
            contributing_actions,
            get_relevant_environmental_factors(env_state),
            tech_id,
            1.0f  // Initial success rate
        });

        // Update species capabilities
        apply_discovery_effects(species_id, tech_id);
    }

    void apply_discovery_effects(const std::string& species_id,
                               const std::string& tech_id) {
        const auto& discovery = potential_discoveries[tech_id];
        
        // Grant new actions
        for (const auto& action : discovery.capabilities.new_actions) {
            grant_action_to_species(species_id, action);
        }

        // Enhance abilities
        for (const auto& ability : discovery.capabilities.enhanced_abilities) {
            enhance_species_ability(species_id, ability);
        }

        // Increase complexity
        increase_species_complexity(
            species_id, 
            discovery.capabilities.complexity_increase
        );
    }
}; 