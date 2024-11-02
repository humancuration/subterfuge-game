#pragma once
#include "SpeciesIdentity.hpp"
#include <unordered_map>

class SpeciesNetworkSystem {
public:
    struct MycelialNetwork {
        enum class NetworkType {
            WOOD_WIDE_WEB,      // Tree-fungal communication network
            CORAL_SYMBIOSIS,    // Coral-algae relationships
            PRAIRIE_NETWORK,    // Grassland plant communications
            URBAN_ADAPTATION    // New networks in city environments
        };

        struct Connection {
            float strength{0.0f};
            float nutrient_flow{0.0f};
            float information_transfer{0.0f};
            std::vector<std::string> shared_signals;
        };

        NetworkType type;
        std::unordered_map<std::string, Connection> connections;
        float network_health{1.0f};
        bool is_teaching_others{false};  // Spreading knowledge to other species
    };

    struct InterspeciesLearning {
        struct Knowledge {
            std::string topic;
            float understanding_level{0.0f};
            std::vector<std::string> practical_applications;
            bool can_teach_others{false};
        };

        std::vector<Knowledge> learned_lessons;
        float teaching_effectiveness{0.0f};
        float learning_rate{0.0f};
    };

private:
    struct CommunityDynamics {
        // Track how species learn from each other
        std::unordered_map<std::string, InterspeciesLearning> species_knowledge;
        
        // Track successful cooperation examples
        struct SuccessStory {
            std::vector<std::string> participating_species;
            std::string lesson_learned;
            float positive_impact;
            bool became_tradition{false};
        };
        std::vector<SuccessStory> cooperation_examples;
    };

public:
    void update_network(float delta_time) {
        // Update mycelial networks
        for (auto& [network_id, network] : mycelial_networks) {
            process_network_communication(network, delta_time);
            share_environmental_knowledge(network);
            adapt_to_changes(network);
        }

        // Process interspecies learning
        for (auto& [species_id, learning] : community.species_knowledge) {
            process_learning_exchange(species_id, learning);
            spread_successful_practices(species_id, learning);
        }
    }

private:
    void process_network_communication(MycelialNetwork& network, float delta_time) {
        // Example: Trees warning each other about pests
        for (auto& [species_id, connection] : network.connections) {
            if (connection.strength > 0.7f) {
                // Share defensive signals
                if (detect_threat(species_id)) {
                    broadcast_warning(network, species_id, "pest_alert");
                }
                
                // Share resources with struggling members
                if (check_resource_needs(species_id)) {
                    share_resources(network, species_id);
                }
            }
        }
    }

    void share_environmental_knowledge(MycelialNetwork& network) {
        if (network.is_teaching_others) {
            for (auto& [species_id, connection] : network.connections) {
                // Teach about soil conditions
                teach_soil_knowledge(species_id, connection);
                
                // Share water finding techniques
                share_water_wisdom(species_id, connection);
                
                // Pass on climate adaptation strategies
                transfer_climate_knowledge(species_id, connection);
            }
        }
    }

    void process_learning_exchange(const std::string& species_id, 
                                 InterspeciesLearning& learning) {
        for (auto& knowledge : learning.learned_lessons) {
            if (knowledge.understanding_level > 0.8f && 
                knowledge.can_teach_others) {
                // Record successful adaptation strategy
                community.cooperation_examples.push_back({
                    {species_id},
                    knowledge.topic,
                    calculate_positive_impact(knowledge),
                    true
                });
                
                // Share with other species
                spread_knowledge(species_id, knowledge);
            }
        }
    }

    void spread_successful_practices(const std::string& species_id,
                                   const InterspeciesLearning& learning) {
        for (const auto& example : community.cooperation_examples) {
            if (example.became_tradition && 
                example.positive_impact > 0.7f) {
                // Teach other species
                teach_successful_practice(species_id, example);
                
                // Document the knowledge transfer
                record_teaching_success(species_id, example);
            }
        }
    }

    float calculate_positive_impact(const InterspeciesLearning::Knowledge& knowledge) {
        float impact = 0.0f;
        
        // Measure environmental benefit
        impact += evaluate_environmental_benefit(knowledge);
        
        // Assess community strengthening
        impact += evaluate_community_benefit(knowledge);
        
        // Consider long-term sustainability
        impact += evaluate_sustainability(knowledge);
        
        return impact / 3.0f;
    }

    void teach_successful_practice(const std::string& teacher_id,
                                 const CommunityDynamics::SuccessStory& practice) {
        // Find suitable students
        auto students = find_receptive_species(teacher_id);
        
        for (const auto& student_id : students) {
            // Adapt teaching method to student
            float effectiveness = adapt_teaching_method(teacher_id, student_id);
            
            // Transfer knowledge
            if (effectiveness > 0.6f) {
                transfer_knowledge(teacher_id, student_id, practice);
                
                // Record successful teaching
                document_teaching_success(teacher_id, student_id, practice);
            }
        }
    }

    std::vector<std::string> find_receptive_species(const std::string& teacher_id) {
        std::vector<std::string> receptive_species;
        
        // Look for species with complementary traits
        for (const auto& [species_id, learning] : community.species_knowledge) {
            if (species_id != teacher_id && 
                learning.learning_rate > 0.5f) {
                receptive_species.push_back(species_id);
            }
        }
        
        return receptive_species;
    }

private:
    std::unordered_map<std::string, MycelialNetwork> mycelial_networks;
    CommunityDynamics community;
}; 