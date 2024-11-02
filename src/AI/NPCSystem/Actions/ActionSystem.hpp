#pragma once
#include "../NPCController.hpp"
#include <unordered_map>
#include <functional>

class ActionSystem {
public:
    struct ActionContext {
        float cultural_influence{0.0f};    // How much culture affects this action
        float environmental_stress{0.0f};  // Current environmental pressure
        float social_pressure{0.0f};       // Influence from other NPCs
        float personal_values{0.0f};       // Individual's values/traits
        float resource_availability{0.0f}; // Available resources
    };

    struct ActionProbability {
        float base_chance{0.0f};
        std::vector<std::string> prerequisites;
        std::function<float(const ActionContext&)> modifier;
    };

    enum class ActionType {
        // Basic survival
        GATHER_RESOURCES,
        SEEK_SHELTER,
        REST,
        
        // Social actions
        SHARE_KNOWLEDGE,
        TRADE_RESOURCES,
        TEACH_OTHERS,
        FORM_ALLIANCE,
        
        // Cultural actions
        PERFORM_RITUAL,
        CREATE_ART,
        SHARE_STORY,
        
        // Environmental actions
        PLANT_TREES,
        CLEAN_AREA,
        REDUCE_CONSUMPTION,
        
        // Innovation actions
        RESEARCH_TECHNOLOGY,
        EXPERIMENT,
        COMBINE_PRACTICES
    };

private:
    std::unordered_map<ActionType, ActionProbability> action_probabilities;
    
    // Track recent actions to avoid repetition
    struct ActionHistory {
        ActionType action;
        float success_rate;
        float impact;
    };
    std::vector<ActionHistory> recent_actions;

public:
    ActionType decide_next_action(NPCController* npc) {
        ActionContext context = gather_context(npc);
        
        // Calculate probabilities for each action
        std::vector<std::pair<ActionType, float>> weighted_actions;
        
        for (const auto& [action, probability] : action_probabilities) {
            if (can_perform_action(npc, action)) {
                float final_probability = calculate_probability(
                    action, probability, context, npc
                );
                weighted_actions.emplace_back(action, final_probability);
            }
        }
        
        return select_weighted_action(weighted_actions);
    }

private:
    ActionContext gather_context(NPCController* npc) {
        ActionContext context;
        
        // Cultural influence
        auto* cultural_id = npc->get_cultural_identity();
        if (cultural_id) {
            context.cultural_influence = cultural_id->get_cultural_pressure();
        }
        
        // Environmental factors
        auto env_state = npc->get_local_environment();
        context.environmental_stress = env_state.is_stressed() ? 1.0f : 0.0f;
        
        // Social pressure from nearby NPCs
        auto nearby_npcs = npc->get_nearby_npcs();
        context.social_pressure = calculate_social_pressure(nearby_npcs);
        
        // Personal values and traits
        context.personal_values = calculate_value_influence(npc);
        
        // Resource state
        context.resource_availability = 
            1.0f - npc->get_resource_consumption();
        
        return context;
    }

    float calculate_probability(
        ActionType action,
        const ActionProbability& prob,
        const ActionContext& context,
        NPCController* npc
    ) {
        float base = prob.base_chance;
        
        // Apply context modifier
        float context_mod = prob.modifier(context);
        
        // Species traits influence
        float species_mod = calculate_species_modifier(npc, action);
        
        // Cultural values influence
        float cultural_mod = calculate_cultural_modifier(npc, action);
        
        // Environmental influence
        float env_mod = calculate_environmental_modifier(npc, action);
        
        // Recent action history influence
        float history_mod = calculate_history_modifier(action);
        
        return base * context_mod * species_mod * cultural_mod * 
               env_mod * history_mod;
    }

    float calculate_species_modifier(NPCController* npc, ActionType action) {
        auto* species = npc->get_species_identity();
        if (!species) return 1.0f;
        
        switch (action) {
            case ActionType::SHARE_KNOWLEDGE:
                return species->has_trait("Teaching_Affinity") ? 1.5f : 1.0f;
            case ActionType::CLEAN_AREA:
                return species->has_trait("Environmental_Care") ? 1.3f : 1.0f;
            // ... other action-specific modifiers
            default:
                return 1.0f;
        }
    }

    float calculate_cultural_modifier(NPCController* npc, ActionType action) {
        auto* culture = npc->get_cultural_identity();
        if (!culture) return 1.0f;
        
        switch (action) {
            case ActionType::PERFORM_RITUAL:
                return culture->values_spirituality() ? 1.4f : 0.6f;
            case ActionType::CREATE_ART:
                return culture->values_creativity() ? 1.3f : 0.7f;
            // ... other cultural influences
            default:
                return 1.0f;
        }
    }

    float calculate_environmental_modifier(NPCController* npc, ActionType action) {
        auto env_state = npc->get_local_environment();
        
        switch (action) {
            case ActionType::SEEK_SHELTER:
                return env_state.climate_stress > 0.7f ? 2.0f : 1.0f;
            case ActionType::REDUCE_CONSUMPTION:
                return env_state.resource_depletion > 0.6f ? 1.5f : 1.0f;
            // ... other environmental influences
            default:
                return 1.0f;
        }
    }

    float calculate_history_modifier(ActionType action) {
        // Reduce probability of recently performed actions
        int recent_count = std::count_if(
            recent_actions.begin(),
            recent_actions.end(),
            [action](const ActionHistory& history) {
                return history.action == action;
            }
        );
        
        return 1.0f / (1.0f + recent_count * 0.2f);
    }

    ActionType select_weighted_action(
        const std::vector<std::pair<ActionType, float>>& weighted_actions
    ) {
        float total_weight = 0.0f;
        for (const auto& [_, weight] : weighted_actions) {
            total_weight += weight;
        }
        
        float random = static_cast<float>(rand()) / RAND_MAX * total_weight;
        
        for (const auto& [action, weight] : weighted_actions) {
            random -= weight;
            if (random <= 0) {
                return action;
            }
        }
        
        return weighted_actions[0].first; // Fallback
    }
}; 