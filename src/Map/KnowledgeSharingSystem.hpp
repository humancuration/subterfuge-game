#pragma once
#include "SpeciesNetworkTypes.hpp"
#include "ResourceSharingNetwork.hpp"
#include <unordered_set>

class KnowledgeSharingSystem {
public:
    struct Knowledge {
        enum class Type {
            ENVIRONMENTAL,    // Understanding of ecosystems
            TECHNOLOGICAL,    // Tool use and innovation
            CULTURAL,        // Social practices and traditions
            SPIRITUAL,       // Philosophical/mystical understanding
            BIOLOGICAL,      // Medical and body knowledge
            AGRICULTURAL,    // Food production techniques
            ARTISTIC,        // Creative expression methods
            DIPLOMATIC      // Inter-species communication
        };

        Type type;
        float understanding_level{0.0f};
        float teaching_efficiency{0.0f};
        bool can_be_taught_across_species{true};
        std::vector<std::string> prerequisites;
        std::vector<std::string> derived_insights;
    };

    struct CulturalPractice {
        std::string name;
        float sustainability_rating{1.0f};
        float cooperation_requirement{0.0f};
        float adaptation_difficulty{0.0f};
        std::vector<std::string> compatible_species;
        std::vector<std::string> incompatible_practices;
    };

    struct LearningChannel {
        enum class Type {
            DIRECT_TEACHING,     // Intentional knowledge transfer
            OBSERVATION,         // Learning by watching
            CULTURAL_OSMOSIS,    // Gradual absorption through exposure
            SHARED_EXPERIENCE,   // Learning through cooperation
            CRISIS_ADAPTATION,   // Learning from shared challenges
            SYMBIOTIC_EXCHANGE,  // Mutual learning through partnership
            COLLECTIVE_MEMORY    // Knowledge preserved in community
        };

        Type type;
        float effectiveness;
        std::vector<std::string> participating_species;
        bool requires_trust;
    };

private:
    struct LearningNetwork {
        std::unordered_map<std::string, std::vector<Knowledge>> species_knowledge;
        std::unordered_map<std::string, std::vector<CulturalPractice>> cultural_practices;
        std::vector<LearningChannel> active_channels;
        
        // Track successful knowledge transfers
        struct KnowledgeTransfer {
            std::string teacher_species;
            std::string student_species;
            Knowledge::Type knowledge_type;
            float success_rate;
            std::vector<std::string> enabling_factors;
        };
        std::vector<KnowledgeTransfer> successful_transfers;
    };

public:
    void update_learning_networks(float delta_time) {
        process_knowledge_sharing(delta_time);
        evolve_cultural_practices(delta_time);
        adapt_learning_channels(delta_time);
    }

private:
    void process_knowledge_sharing(float delta_time) {
        for (auto& channel : learning_network.active_channels) {
            // Process each learning channel
            for (size_t i = 0; i < channel.participating_species.size(); ++i) {
                for (size_t j = i + 1; j < channel.participating_species.size(); ++j) {
                    const auto& species1 = channel.participating_species[i];
                    const auto& species2 = channel.participating_species[j];
                    
                    if (can_share_knowledge(species1, species2, channel)) {
                        share_knowledge_between_species(
                            species1, species2, channel, delta_time);
                    }
                }
            }
        }
    }

    void share_knowledge_between_species(
        const std::string& species1,
        const std::string& species2,
        const LearningChannel& channel,
        float delta_time) {
        
        // Calculate base learning rate
        float learning_rate = calculate_learning_rate(species1, species2, channel);
        
        // Share each type of knowledge
        for (const auto& knowledge : learning_network.species_knowledge[species1]) {
            if (knowledge.can_be_taught_across_species) {
                float transfer_success = attempt_knowledge_transfer(
                    species1, species2, knowledge, learning_rate);
                
                if (transfer_success > 0.5f) {
                    // Record successful transfer
                    learning_network.successful_transfers.push_back({
                        species1, species2, knowledge.type, transfer_success,
                        get_enabling_factors(species1, species2)
                    });
                    
                    // Potentially develop new insights
                    check_for_new_insights(species2, knowledge);
                }
            }
        }
    }

    void evolve_cultural_practices(float delta_time) {
        for (auto& [species, practices] : learning_network.cultural_practices) {
            for (auto& practice : practices) {
                // Adapt practices based on environment
                float adaptation_need = calculate_adaptation_need(species, practice);
                
                if (adaptation_need > 0.7f) {
                    // Evolve the practice
                    evolve_practice(practice, species);
                    
                    // Share evolved practice with compatible species
                    for (const auto& compatible : practice.compatible_species) {
                        share_evolved_practice(practice, species, compatible);
                    }
                }
            }
        }
    }

    void check_for_new_insights(const std::string& species, 
                               const Knowledge& learned_knowledge) {
        auto& species_knowledge = learning_network.species_knowledge[species];
        
        // Check if new combinations of knowledge enable insights
        for (const auto& existing : species_knowledge) {
            if (can_generate_insight(learned_knowledge, existing)) {
                Knowledge new_insight = generate_new_insight(
                    learned_knowledge, existing);
                
                species_knowledge.push_back(new_insight);
                
                // This new insight might enable new cultural practices
                check_for_new_practices(species, new_insight);
            }
        }
    }

    bool can_generate_insight(const Knowledge& k1, const Knowledge& k2) {
        // Different knowledge types more likely to generate insights
        if (k1.type != k2.type && 
            k1.understanding_level > 0.7f && 
            k2.understanding_level > 0.7f) {
            return true;
        }
        return false;
    }

    Knowledge generate_new_insight(const Knowledge& k1, const Knowledge& k2) {
        Knowledge insight;
        insight.type = determine_insight_type(k1, k2);
        insight.understanding_level = 0.1f;  // Start low
        insight.teaching_efficiency = 0.5f;
        insight.prerequisites = {k1.name, k2.name};
        return insight;
    }

private:
    LearningNetwork learning_network;
}; 