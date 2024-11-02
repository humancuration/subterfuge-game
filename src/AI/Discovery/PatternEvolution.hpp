#pragma once
#include "PatternRecognitionSystem.hpp"
#include <random>

class PatternEvolution {
public:
    struct EvolutionResult {
        Pattern evolved_pattern;
        float innovation_score{0.0f};
        bool is_novel{false};
        std::vector<std::string> new_elements;
    };

    struct CombinationResult {
        Pattern combined_pattern;
        float synergy_score{0.0f};
        std::vector<std::string> emergent_properties;
    };

private:
    // Random number generation
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist{0.0f, 1.0f};

    // Evolution parameters
    struct EvolutionParams {
        float mutation_rate{0.1f};
        float combination_rate{0.2f};
        float innovation_threshold{0.7f};
        float synergy_threshold{0.6f};
    } params;

public:
    EvolutionResult evolve_pattern(const Pattern& pattern) {
        EvolutionResult result;
        result.evolved_pattern = pattern;

        // Attempt mutations
        if (dist(rng) < params.mutation_rate) {
            mutate_pattern(result.evolved_pattern);
            result.innovation_score = calculate_innovation(
                pattern, result.evolved_pattern);
        }

        // Check for novel elements
        result.is_novel = result.innovation_score > params.innovation_threshold;
        if (result.is_novel) {
            result.new_elements = find_new_elements(
                pattern, result.evolved_pattern);
        }

        return result;
    }

    CombinationResult combine_patterns(
        const Pattern& p1, const Pattern& p2) {
        CombinationResult result;

        // Base combination
        result.combined_pattern = merge_base_patterns(p1, p2);

        // Check for synergies
        result.synergy_score = calculate_synergy(p1, p2);
        if (result.synergy_score > params.synergy_threshold) {
            generate_emergent_properties(result);
        }

        return result;
    }

private:
    void mutate_pattern(Pattern& pattern) {
        // Possible mutations:
        // 1. Add new action
        if (dist(rng) < 0.3f) {
            add_random_action(pattern);
        }

        // 2. Modify existing action
        if (dist(rng) < 0.3f) {
            modify_random_action(pattern);
        }

        // 3. Change environmental factors
        if (dist(rng) < 0.3f) {
            mutate_environmental_factors(pattern);
        }

        // 4. Evolve cultural elements
        if (dist(rng) < 0.3f) {
            evolve_cultural_elements(pattern);
        }
    }

    void add_random_action(Pattern& pattern) {
        // Add complementary action based on existing ones
        std::vector<std::string> potential_actions = 
            get_complementary_actions(pattern.actions);
        
        if (!potential_actions.empty()) {
            size_t idx = std::uniform_int_distribution<size_t>{
                0, potential_actions.size() - 1}(rng);
            pattern.actions.push_back(potential_actions[idx]);
        }
    }

    void modify_random_action(Pattern& pattern) {
        if (pattern.actions.empty()) return;

        size_t idx = std::uniform_int_distribution<size_t>{
            0, pattern.actions.size() - 1}(rng);
        
        pattern.actions[idx] = evolve_action(pattern.actions[idx]);
    }

    Pattern merge_base_patterns(const Pattern& p1, const Pattern& p2) {
        Pattern merged;

        // Merge actions with potential synergies
        merged.actions = merge_action_sets(p1.actions, p2.actions);

        // Combine environmental factors
        merged.environmental_factors = 
            merge_environmental_factors(
                p1.environmental_factors, 
                p2.environmental_factors
            );

        // Synthesize cultural elements
        merged.cultural_elements = 
            synthesize_cultural_elements(
                p1.cultural_elements, 
                p2.cultural_elements
            );

        return merged;
    }

    float calculate_synergy(const Pattern& p1, const Pattern& p2) {
        float action_synergy = calculate_action_synergy(
            p1.actions, p2.actions);
        
        float cultural_synergy = calculate_cultural_synergy(
            p1.cultural_elements, p2.cultural_elements);
        
        float environmental_compatibility = 
            calculate_environmental_compatibility(
                p1.environmental_factors, 
                p2.environmental_factors
            );

        return (action_synergy + cultural_synergy + 
                environmental_compatibility) / 3.0f;
    }

    void generate_emergent_properties(CombinationResult& result) {
        // Check for emergent properties based on pattern combinations
        if (has_tool_synergy(result.combined_pattern)) {
            result.emergent_properties.push_back("advanced_tool_use");
        }

        if (has_cultural_synergy(result.combined_pattern)) {
            result.emergent_properties.push_back("cultural_innovation");
        }

        if (has_environmental_synergy(result.combined_pattern)) {
            result.emergent_properties.push_back("environmental_adaptation");
        }

        // Apply emergent properties to pattern
        apply_emergent_properties(
            result.combined_pattern, 
            result.emergent_properties
        );
    }

    std::vector<std::string> merge_action_sets(
        const std::vector<std::string>& actions1,
        const std::vector<std::string>& actions2) {
        std::vector<std::string> merged;
        std::set_union(
            actions1.begin(), actions1.end(),
            actions2.begin(), actions2.end(),
            std::back_inserter(merged)
        );

        // Look for potential combinations
        for (size_t i = 0; i < actions1.size(); ++i) {
            for (size_t j = 0; j < actions2.size(); ++j) {
                if (auto combined = try_combine_actions(
                    actions1[i], actions2[j])) {
                    merged.push_back(*combined);
                }
            }
        }

        return merged;
    }

    std::optional<std::string> try_combine_actions(
        const std::string& a1, const std::string& a2) {
        // Check if actions can be combined into a new, more advanced action
        if (are_actions_complementary(a1, a2)) {
            return generate_combined_action(a1, a2);
        }
        return std::nullopt;
    }
}; 