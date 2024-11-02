#pragma once
#include "../BehaviorTrees/BehaviorNode.hpp"
#include "../NPCController.hpp"
#include "../Knowledge.hpp"
#include <algorithm>

class ShareKnowledgeBehavior : public BehaviorNode {
public:
    Status execute(NPCController* npc) override {
        auto nearby_npcs = npc->get_nearby_npcs();
        
        for (auto* other_npc : nearby_npcs) {
            if (can_share_knowledge(npc, other_npc)) {
                auto sharable_knowledge = npc->get_sharable_knowledge();
                
                for (const auto& knowledge : sharable_knowledge) {
                    if (npc->share_knowledge(other_npc, knowledge)) {
                        return Status::SUCCESS;
                    }
                }
            }
        }
        
        return Status::FAILURE;
    }

private:
    bool can_share_knowledge(NPCController* sharer, NPCController* receiver) {
        auto* sharer_culture = sharer->get_cultural_identity();
        auto* receiver_culture = receiver->get_cultural_identity();
        
        if (!sharer_culture || !receiver_culture) return false;
        
        // Check cultural compatibility
        float compatibility = calculate_compatibility(
            sharer_culture,
            receiver_culture
        );
        
        return compatibility > 0.5f;
    }

    float calculate_compatibility(CulturalIdentity* c1, CulturalIdentity* c2) {
        // Base compatibility on shared values and practices
        float shared_values = count_shared_values(c1, c2);
        float practice_overlap = calculate_practice_overlap(c1, c2);
        
        return (shared_values + practice_overlap) / 2.0f;
    }

    float count_shared_values(CulturalIdentity* c1, CulturalIdentity* c2) {
        // Implementation depends on CulturalIdentity interface
        return 0.5f; // Placeholder
    }

    float calculate_practice_overlap(CulturalIdentity* c1, CulturalIdentity* c2) {
        // Implementation depends on CulturalIdentity interface
        return 0.5f; // Placeholder
    }
}; 