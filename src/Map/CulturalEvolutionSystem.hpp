#pragma once
#include "KnowledgeSharingSystem.hpp"
#include <unordered_set>

class CulturalEvolutionSystem {
public:
    struct CulturalPractice {
        enum class Category {
            SUSTAINABILITY,    // Environmental practices
            SOCIAL,           // Community organization
            SPIRITUAL,        // Beliefs and rituals
            TECHNOLOGICAL,    // Tool use and innovation
            ARTISTIC,         // Creative expression
            EDUCATIONAL,      // Knowledge transfer
            DIPLOMATIC,       // Inter-species relations
            ECONOMIC         // Resource management
        };

        std::string name;
        Category category;
        float effectiveness{1.0f};
        float adoption_rate{0.0f};
        std::vector<std::string> prerequisites;
        std::vector<std::string> synergistic_practices;
        bool requires_advanced_cognition{false};
    };

    struct CulturalMeme {
        std::string idea;
        float virality{0.0f};
        float staying_power{1.0f};
        std::vector<std::string> related_practices;
        bool challenges_status_quo{false};
    };

    struct Innovation {
        enum class Source {
            NECESSITY,           // Problem-solving
            CULTURAL_FUSION,     // Mixing different practices
            ACCIDENT,            // Serendipitous discovery
            OBSERVATION,         // Learning from nature
            EXPERIMENTATION,     // Systematic testing
            SPIRITUAL_INSIGHT    // Meditation/reflection
        };

        Source origin;
        float revolutionary_factor{0.0f};
        std::vector<std::string> inspired_by;
        bool changes_power_dynamics{false};
    };

private:
    struct CulturalGroup {
        std::vector<CulturalPractice> active_practices;
        std::vector<CulturalMeme> circulating_memes;
        float cultural_momentum{0.0f};
        float resistance_to_change{0.5f};
        float innovation_rate{0.5f};
        
        // Track cultural exchanges
        struct Exchange {
            std::string with_group;
            float success_rate;
            std::vector<std::string> shared_practices;
        };
        std::vector<Exchange> cultural_exchanges;
    };

    struct EmergentTrend {
        std::vector<std::string> contributing_practices;
        float societal_impact;
        bool is_sustainable;
        std::string resulting_innovation;
    };

public:
    void update_cultural_evolution(float delta_time) {
        process_cultural_exchanges(delta_time);
        evolve_practices(delta_time);
        check_for_innovations();
        spread_successful_memes();
    }

private:
    void process_cultural_exchanges(float delta_time) {
        for (auto& [group_name, group] : cultural_groups) {
            for (auto& exchange : group.cultural_exchanges) {
                // Calculate exchange success based on multiple factors
                float success_chance = calculate_exchange_success(
                    group,
                    cultural_groups[exchange.with_group],
                    delta_time
                );

                if (should_exchange_succeed(success_chance)) {
                    facilitate_cultural_exchange(group, exchange);
                }
            }
        }
    }

    void evolve_practices(float delta_time) {
        for (auto& [group_name, group] : cultural_groups) {
            for (auto& practice : group.active_practices) {
                // Natural evolution of practices
                float evolution_rate = calculate_evolution_rate(
                    practice,
                    group.cultural_momentum,
                    delta_time
                );

                if (should_practice_evolve(evolution_rate)) {
                    evolve_practice(practice, group);
                }

                // Check for practice combinations
                check_practice_synergies(group, practice);
            }
        }
    }

    void check_for_innovations() {
        for (auto& [group_name, group] : cultural_groups) {
            if (group.innovation_rate > random_float()) {
                // Different innovation sources
                for (const auto source : {
                    Innovation::Source::NECESSITY,
                    Innovation::Source::CULTURAL_FUSION,
                    Innovation::Source::OBSERVATION
                }) {
                    attempt_innovation(group, source);
                }
            }
        }
    }

    void attempt_innovation(CulturalGroup& group, Innovation::Source source) {
        // Calculate innovation chance based on source
        float chance = calculate_innovation_chance(group, source);
        
        if (random_float() < chance) {
            Innovation innovation{source};
            
            // Determine innovation characteristics
            innovation.revolutionary_factor = calculate_revolutionary_factor(
                group.active_practices,
                source
            );
            
            // Apply innovation effects
            apply_innovation_effects(group, innovation);
            
            // Record the innovation
            record_cultural_breakthrough(group, innovation);
        }
    }

    void spread_successful_memes() {
        for (auto& [group_name, group] : cultural_groups) {
            for (auto& meme : group.circulating_memes) {
                if (meme.virality > random_float()) {
                    // Spread to other groups
                    spread_meme_to_neighbors(group, meme);
                    
                    // Possibly evolve into practice
                    if (should_become_practice(meme)) {
                        convert_meme_to_practice(group, meme);
                    }
                }
            }
        }
    }

    void convert_meme_to_practice(CulturalGroup& group, const CulturalMeme& meme) {
        CulturalPractice practice;
        practice.name = "Practice_" + meme.idea;
        practice.effectiveness = meme.staying_power;
        practice.adoption_rate = meme.virality;
        
        // Add prerequisites based on existing practices
        for (const auto& related : meme.related_practices) {
            if (has_practice(group, related)) {
                practice.prerequisites.push_back(related);
            }
        }
        
        group.active_practices.push_back(practice);
    }

    bool should_become_practice(const CulturalMeme& meme) {
        return meme.staying_power > 0.8f && 
               meme.virality > 0.6f;
    }

private:
    std::unordered_map<std::string, CulturalGroup> cultural_groups;
    std::vector<EmergentTrend> observed_trends;
}; 