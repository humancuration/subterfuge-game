#pragma once
#include "SpeciesNetworkTypes.hpp"
#include <unordered_map>

class ResourceSharingNetwork {
public:
    struct RelatednessFactor {
        float genetic_similarity{0.0f};    // Biological relatedness
        float cultural_overlap{0.0f};      // Shared cultural practices
        float historical_bonds{0.0f};      // Historical relationships
        float mutual_understanding{0.0f};  // Education/empathy level
    };

    struct SharingBehavior {
        enum class Type {
            PURE_ALTRUISM,        // Shares without expectation
            KIN_SELECTION,        // Favors genetic relatives
            RECIPROCAL,           // Expects return sharing
            CULTURAL_BONDING,     // Based on shared values
            ENLIGHTENED_SHARING,  // Education-driven cooperation
            CRISIS_RESPONSE,      // Emergency resource sharing
            ABUNDANCE_SHARING     // Shares excess resources
        };

        Type behavior_type;
        float sharing_threshold;  // How much resource difference triggers sharing
        float sharing_rate;       // How much is shared
        bool is_conditional;      // Whether sharing depends on reciprocation
    };

    struct ResourceState {
        float abundance_level{1.0f};
        float perceived_scarcity{0.0f};
        float distribution_equity{1.0f};
        bool is_sustainable{true};
    };

private:
    struct CommunityDynamics {
        float education_level{0.5f};
        float resource_stress{0.0f};
        float cooperation_index{0.5f};
        float cultural_diversity{0.5f};
        
        // Tracks how different groups interact
        std::unordered_map<std::string, std::unordered_map<std::string, float>> 
            intergroup_trust;
    };

    struct AdaptiveBehavior {
        float learning_rate{0.1f};
        float prejudice_reduction{0.0f};
        float empathy_growth{0.0f};
        std::vector<std::string> learned_lessons;
    };

public:
    void update_sharing_dynamics(float delta_time) {
        // Update resource distribution
        update_resource_flows(delta_time);
        
        // Process learning and adaptation
        process_social_learning(delta_time);
        
        // Update community relationships
        update_relationship_networks(delta_time);
    }

private:
    void update_resource_flows(float delta_time) {
        for (auto& [group_id, resources] : group_resources) {
            // Calculate sharing based on relatedness and conditions
            float sharing_amount = calculate_sharing_amount(
                group_id,
                resources.abundance_level,
                community.resource_stress
            );

            // Modify sharing based on education and understanding
            sharing_amount *= (1.0f + community.education_level);

            // Apply cultural bonding effects
            if (has_cultural_connection(group_id)) {
                sharing_amount *= 1.2f;  // 20% bonus for cultural bonds
            }

            distribute_resources(group_id, sharing_amount);
        }
    }

    void process_social_learning(float delta_time) {
        for (auto& [group_id, behavior] : group_behaviors) {
            // Education reduces in-group bias
            if (community.education_level > 0.7f) {
                behavior.prejudice_reduction += delta_time * 0.1f;
            }

            // Successful sharing increases trust
            if (check_successful_sharing(group_id)) {
                increase_intergroup_trust(group_id);
            }

            // Resource abundance promotes generosity
            if (check_resource_abundance()) {
                behavior.empathy_growth += delta_time * 0.05f;
            }

            // Document successful cooperation
            if (behavior.empathy_growth > 0.5f) {
                behavior.learned_lessons.push_back(
                    "Cooperation benefits everyone"
                );
            }
        }
    }

    void update_relationship_networks(float delta_time) {
        for (auto& [group1, relations] : community.intergroup_trust) {
            for (auto& [group2, trust_level] : relations) {
                // Shared hardship can build bonds
                if (community.resource_stress > 0.7f) {
                    trust_level += delta_time * 0.1f * 
                        community.cooperation_index;
                }

                // Cultural exchange increases understanding
                if (has_cultural_exchange(group1, group2)) {
                    trust_level += delta_time * 0.05f;
                }

                // Education promotes tolerance
                if (community.education_level > 0.8f) {
                    trust_level = std::min(1.0f, trust_level + 
                        delta_time * 0.02f);
                }
            }
        }
    }

    bool has_cultural_connection(const std::string& group_id) {
        return cultural_connections.contains(group_id) && 
               cultural_connections[group_id].cultural_overlap > 0.5f;
    }

    void increase_intergroup_trust(const std::string& group_id) {
        for (auto& [other_group, trust_level] : 
             community.intergroup_trust[group_id]) {
            // Successful cooperation builds trust gradually
            trust_level = std::min(1.0f, trust_level + 0.01f);
        }
    }

    bool check_resource_abundance() {
        float total_abundance = 0.0f;
        for (const auto& [_, resources] : group_resources) {
            total_abundance += resources.abundance_level;
        }
        return (total_abundance / group_resources.size()) > 0.7f;
    }

private:
    CommunityDynamics community;
    std::unordered_map<std::string, ResourceState> group_resources;
    std::unordered_map<std::string, AdaptiveBehavior> group_behaviors;
    std::unordered_map<std::string, RelatednessFactor> cultural_connections;
}; 