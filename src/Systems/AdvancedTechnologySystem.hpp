#pragma once
#include "../AI/Discovery/EmergentTechnologySystem.hpp"
#include "../AI/Discovery/PatternRecognitionSystem.hpp"
#include <memory>

class AdvancedTechnologySystem {
private:
    struct TechnologyAdvancement {
        enum class AdvancementType {
            EMERGENT_DISCOVERY,    // Natural/cultural discovery
            DIRECTED_RESEARCH,     // Funded research
            COLLABORATIVE_INSIGHT, // Multiple species working together
            CRISIS_INNOVATION     // Advancement due to environmental pressure
        };

        std::string tech_id;
        AdvancementType type;
        float progress{0.0f};
        float funding{0.0f};
        std::vector<std::string> contributing_species;
        
        struct Factors {
            float cultural_readiness{0.0f};
            float resource_availability{0.0f};
            float environmental_pressure{0.0f};
            float knowledge_base{0.0f};
        } factors;
    };

    std::unique_ptr<EmergentTechnologySystem> emergence_system;
    std::unique_ptr<PatternRecognitionSystem> pattern_system;
    std::unordered_map<std::string, TechnologyAdvancement> advancements;

public:
    void process_advancement(float delta_time) {
        // Process both emergent discoveries and directed research
        process_emergent_discoveries();
        process_research_projects();
        process_crisis_innovations();
    }

    void allocate_funding(const std::string& tech_id, float amount) {
        if (auto it = advancements.find(tech_id); it != advancements.end()) {
            it->second.funding += amount;
            update_research_factors(it->second);
        }
    }

    void register_observation(const std::string& species_id, 
                            const std::vector<std::string>& actions,
                            const EnvironmentalState& env_state) {
        // Let both systems process the observation
        emergence_system->check_for_discovery(species_id, actions, env_state);
        pattern_system->observe_success(actions, 
                                      env_state.get_factors(),
                                      get_cultural_elements(species_id),
                                      "");
    }

private:
    void process_emergent_discoveries() {
        // Process natural discoveries through pattern recognition
        for (auto& advancement : advancements) {
            if (advancement.second.type == TechnologyAdvancement::AdvancementType::EMERGENT_DISCOVERY) {
                process_emergence(advancement.second);
            }
        }
    }

    void process_research_projects() {
        // Process funded research projects
        for (auto& advancement : advancements) {
            if (advancement.second.type == TechnologyAdvancement::AdvancementType::DIRECTED_RESEARCH) {
                process_research(advancement.second);
            }
        }
    }

    void process_emergence(TechnologyAdvancement& advancement) {
        // Calculate emergence factors
        float emergence_chance = calculate_emergence_chance(advancement);
        
        if (random_float() < emergence_chance) {
            trigger_discovery(advancement);
        }
    }

    void process_research(TechnologyAdvancement& advancement) {
        // Progress based on funding and other factors
        float progress_rate = calculate_progress_rate(advancement);
        advancement.progress += progress_rate;
        
        if (advancement.progress >= 1.0f) {
            complete_research(advancement);
        }
    }

    float calculate_emergence_chance(const TechnologyAdvancement& advancement) {
        return (advancement.factors.cultural_readiness +
                advancement.factors.environmental_pressure +
                advancement.factors.knowledge_base) / 3.0f;
    }

    float calculate_progress_rate(const TechnologyAdvancement& advancement) {
        return (advancement.funding * 0.5f +
                advancement.factors.resource_availability * 0.3f +
                advancement.factors.knowledge_base * 0.2f);
    }
}; 