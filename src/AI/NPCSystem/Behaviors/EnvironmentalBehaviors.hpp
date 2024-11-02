#pragma once
#include "../BehaviorTrees/BehaviorNode.hpp"
#include "../../Map/EnvironmentalInfluenceSystem.hpp"

class EnvironmentalAwarenessBehavior : public BehaviorNode {
public:
    Status execute(NPCController* npc) override {
        // Check local environment
        auto environmental_state = npc->get_local_environment();
        
        // React to environmental conditions
        if (environmental_state.pollution_level > 0.7f) {
            // Take action to reduce pollution
            npc->adopt_sustainable_practice();
            return Status::SUCCESS;
        }
        
        // Monitor resource usage
        if (npc->get_resource_consumption() > sustainable_threshold) {
            npc->reduce_resource_consumption();
            return Status::SUCCESS;
        }
        
        return Status::RUNNING;
    }

private:
    float sustainable_threshold{0.8f};
}; 