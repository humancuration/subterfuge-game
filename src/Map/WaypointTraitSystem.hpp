#pragma once
#include "WaypointEvolutionProcessor.hpp"
#include <unordered_set>
#include <random>

class WaypointTraitSystem {
private:
    struct TraitInteraction {
        alignas(16) float synergy_strength;
        alignas(16) float mutation_chance;
        std::string result_trait;
        std::function<void(Waypoint*)> interaction_effect;
    };

    struct TraitCategory {
        enum class Type {
            CULTURAL,    // Influences social development
            ECONOMIC,    // Affects resource management
            MILITARY,    // Combat and defense
            SCIENTIFIC,  // Research and technology
            SPIRITUAL,   // Religious and philosophical
            ECOLOGICAL   // Environmental interaction
        };
    };

    // SIMD-aligned trait interaction data
    alignas(32) std::vector<float> trait_strengths;
    alignas(32) std::vector<float> interaction_potentials;
    
    std::unordered_map<std::string, std::unordered_map<std::string, TraitInteraction>> trait_interactions;
    std::unordered_map<std::string, TraitCategory::Type> trait_categories;

public:
    void initialize_trait_system() {
        // Cultural Traits
        add_trait("Artistic Expression", TraitCategory::Type::CULTURAL, [](Waypoint* w) {
            w->modify_stat("CulturalInfluence", 1.2f);
            w->add_building_type("Art Studio");
            w->increase_tourism_appeal(0.3f);
        });

        add_trait("Philosophical Tradition", TraitCategory::Type::SPIRITUAL, [](Waypoint* w) {
            w->modify_stat("Education", 1.15f);
            w->modify_stat("SocialStability", 1.1f);
            w->add_special_status("Center of Learning");
        });

        // Economic Traits
        add_trait("Market Innovation", TraitCategory::Type::ECONOMIC, [](Waypoint* w) {
            w->modify_stat("TradeEfficiency", 1.25f);
            w->add_building_type("Trading Hub");
            w->increase_resource_generation(0.2f);
        });

        // Define trait interactions
        add_trait_interaction("Artistic Expression", "Market Innovation", {
            1.5f,  // Strong synergy
            0.2f,  // Moderate mutation chance
            "Cultural Market",
            [](Waypoint* w) {
                w->evolve_to_specialized_state("Arts District");
                w->add_unique_capability("Cultural Exports");
                w->modify_stat("EconomicProsperity", 1.3f);
                w->modify_stat("CulturalInfluence", 1.4f);
            }
        });

        add_trait_interaction("Philosophical Tradition", "Scientific Method", {
            1.8f,  // Very strong synergy
            0.3f,  // High mutation chance
            "Enlightened Society",
            [](Waypoint* w) {
                w->evolve_to_specialized_state("Center of Enlightenment");
                w->add_research_bonus("Philosophy", 2.0f);
                w->add_research_bonus("Science", 2.0f);
                w->modify_stat("Education", 1.5f);
                w->spawn_unique_building("Grand Academy");
            }
        });

        // Environmental Adaptations
        add_trait("Ecological Harmony", TraitCategory::Type::ECOLOGICAL, [](Waypoint* w) {
            w->modify_stat("EnvironmentalHealth", 1.3f);
            w->add_resource_type("Sustainable Resources");
            w->modify_stat("PopulationHappiness", 1.1f);
        });

        // Military Innovations
        add_trait("Strategic Defense", TraitCategory::Type::MILITARY, [](Waypoint* w) {
            w->modify_stat("DefenseStrength", 1.4f);
            w->add_building_type("Fortification");
            w->increase_territorial_control(0.2f);
        });

        // Create complex interactions between multiple traits
        add_multi_trait_interaction({"Ecological Harmony", "Strategic Defense", "Market Innovation"}, {
            2.0f,  // Very strong synergy
            0.15f, // Controlled mutation chance
            "Sustainable Fortress City",
            [](Waypoint* w) {
                w->evolve_to_advanced_state("Eco-Military Complex");
                w->add_unique_capability("Sustainable Defense Industry");
                w->modify_stat("ResourceEfficiency", 1.5f);
                w->modify_stat("DefenseStrength", 1.6f);
                w->modify_stat("EnvironmentalHealth", 1.4f);
                w->spawn_unique_building("Green Military Academy");
            }
        });
    }

    void process_trait_evolution(Waypoint* waypoint, float delta_time) {
        const size_t trait_count = trait_strengths.size();
        
        // Load environmental and social factors using SIMD
        __m256 environment = _mm256_set1_ps(waypoint->get_stat("EnvironmentalHealth"));
        __m256 social = _mm256_set1_ps(waypoint->get_stat("SocialStability"));
        __m256 economic = _mm256_set1_ps(waypoint->get_stat("EconomicProsperity"));
        
        for (size_t i = 0; i < trait_count; i += 8) {
            // Process trait evolution in SIMD batches
            __m256 strengths = _mm256_load_ps(&trait_strengths[i]);
            __m256 potentials = _mm256_load_ps(&interaction_potentials[i]);
            
            // Calculate evolution factors
            __m256 evolution = _mm256_mul_ps(
                _mm256_mul_ps(environment, social),
                _mm256_mul_ps(economic, potentials)
            );
            
            // Apply evolution
            strengths = _mm256_add_ps(strengths, evolution);
            _mm256_store_ps(&trait_strengths[i], strengths);
            
            // Check for trait interactions
            check_trait_interactions(waypoint, i);
        }
    }

private:
    void check_trait_interactions(Waypoint* waypoint, size_t trait_index) {
        // Check for potential trait combinations
        const auto& active_traits = waypoint->get_active_traits();
        
        for (const auto& trait1 : active_traits) {
            for (const auto& trait2 : active_traits) {
                if (trait1 != trait2) {
                    auto it = trait_interactions.find(trait1);
                    if (it != trait_interactions.end()) {
                        auto interaction_it = it->second.find(trait2);
                        if (interaction_it != it->second.end()) {
                            // Roll for interaction
                            static thread_local std::mt19937 gen{std::random_device{}()};
                            std::uniform_real_distribution<float> dis(0.0f, 1.0f);
                            
                            if (dis(gen) < interaction_it->second.mutation_chance) {
                                interaction_it->second.interaction_effect(waypoint);
                            }
                        }
                    }
                }
            }
        }
    }
}; 