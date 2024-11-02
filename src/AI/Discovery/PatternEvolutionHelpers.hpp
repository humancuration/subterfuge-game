#pragma once
#include "PatternEvolution.hpp"
#include "../Core/RandomGenerator.hpp"
#include <optional>

class PatternEvolutionHelpers {
public:
    // Core pattern manipulation
    static std::vector<std::string> get_complementary_actions(
        const std::vector<std::string>& current_actions) {
        std::vector<std::string> complementary;
        
        // Look for potential synergies
        for (const auto& action : current_actions) {
            if (action.find("tool_use") != std::string::npos) {
                complementary.push_back("tool_creation");
                complementary.push_back("tool_modification");
            }
            if (action.find("observe") != std::string::npos) {
                complementary.push_back("experiment");
                complementary.push_back("analyze");
            }
            if (action.find("communicate") != std::string::npos) {
                complementary.push_back("teach");
                complementary.push_back("coordinate");
            }
        }
        
        return complementary;
    }

    static std::string evolve_action(const std::string& action) {
        // Action evolution patterns
        if (action == "observe") return "analyze";
        if (action == "analyze") return "experiment";
        if (action == "experiment") return "innovate";
        
        if (action == "use_tool") return "modify_tool";
        if (action == "modify_tool") return "create_tool";
        if (action == "create_tool") return "innovate_tool";
        
        if (action == "gather") return "cultivate";
        if (action == "cultivate") return "farm";
        if (action == "farm") return "automate_farming";
        
        return action; // No evolution path found
    }

    static bool are_actions_complementary(
        const std::string& a1, 
        const std::string& a2) {
        // Define complementary pairs
        static const std::unordered_map<std::string, std::vector<std::string>> 
            complementary_pairs = {
                {"observe", {"experiment", "analyze"}},
                {"create", {"modify", "improve"}},
                {"gather", {"process", "store"}},
                {"communicate", {"coordinate", "teach"}},
                {"explore", {"map", "settle"}}
            };
        
        // Check if actions complement each other
        for (const auto& [base, complements] : complementary_pairs) {
            if (a1.find(base) != std::string::npos) {
                for (const auto& complement : complements) {
                    if (a2.find(complement) != std::string::npos) {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }

    static std::string generate_combined_action(
        const std::string& a1, 
        const std::string& a2) {
        // Create advanced actions from combinations
        if (has_both(a1, a2, "observe", "experiment")) 
            return "scientific_method";
        if (has_both(a1, a2, "create", "modify")) 
            return "innovation";
        if (has_both(a1, a2, "gather", "process")) 
            return "resource_management";
        if (has_both(a1, a2, "communicate", "coordinate")) 
            return "social_organization";
        
        // Default combination
        return a1 + "_with_" + a2;
    }

    // Pattern analysis
    static bool has_tool_synergy(const Pattern& pattern) {
        return has_action_type(pattern, "tool") && 
               has_action_type(pattern, "create");
    }

    static bool has_cultural_synergy(const Pattern& pattern) {
        return has_action_type(pattern, "communicate") && 
               has_action_type(pattern, "teach");
    }

    static bool has_environmental_synergy(const Pattern& pattern) {
        return has_action_type(pattern, "observe") && 
               has_action_type(pattern, "modify");
    }

    static void apply_emergent_properties(
        Pattern& pattern,
        const std::vector<std::string>& properties) {
        for (const auto& property : properties) {
            if (property == "advanced_tool_use") {
                pattern.actions.push_back("create_complex_tools");
                pattern.cultural_elements.push_back("tool_tradition");
            }
            else if (property == "cultural_innovation") {
                pattern.actions.push_back("teach_innovations");
                pattern.cultural_elements.push_back("innovation_culture");
            }
            else if (property == "environmental_adaptation") {
                pattern.actions.push_back("modify_environment");
                pattern.environmental_factors.push_back("sustainable_practice");
            }
        }
    }

    // Innovation tracking
    static float calculate_innovation(
        const Pattern& original, 
        const Pattern& evolved) {
        float new_elements = count_new_elements(original, evolved);
        float total_elements = evolved.actions.size() + 
                             evolved.cultural_elements.size() + 
                             evolved.environmental_factors.size();
        
        return new_elements / total_elements;
    }

    static std::vector<std::string> find_new_elements(
        const Pattern& original,
        const Pattern& evolved) {
        std::vector<std::string> new_elements;
        
        // Check for new actions
        for (const auto& action : evolved.actions) {
            if (!contains(original.actions, action)) {
                new_elements.push_back(action);
            }
        }
        
        // Check for new cultural elements
        for (const auto& element : evolved.cultural_elements) {
            if (!contains(original.cultural_elements, element)) {
                new_elements.push_back(element);
            }
        }
        
        return new_elements;
    }

private:
    static bool has_action_type(
        const Pattern& pattern,
        const std::string& type) {
        for (const auto& action : pattern.actions) {
            if (action.find(type) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    static bool has_both(
        const std::string& a1,
        const std::string& a2,
        const std::string& type1,
        const std::string& type2) {
        return (a1.find(type1) != std::string::npos && 
                a2.find(type2) != std::string::npos) ||
               (a1.find(type2) != std::string::npos && 
                a2.find(type1) != std::string::npos);
    }

    static float count_new_elements(
        const Pattern& original,
        const Pattern& evolved) {
        float count = 0;
        
        for (const auto& action : evolved.actions) {
            if (!contains(original.actions, action)) count++;
        }
        
        for (const auto& element : evolved.cultural_elements) {
            if (!contains(original.cultural_elements, element)) count++;
        }
        
        return count;
    }

    template<typename T>
    static bool contains(
        const std::vector<T>& vec,
        const T& item) {
        return std::find(vec.begin(), vec.end(), item) != vec.end();
    }
}; 