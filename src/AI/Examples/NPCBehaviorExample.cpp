#include "../BehaviorTrees/BehaviorTreeBuilder.hpp"

BehaviorNodePtr create_npc_behavior() {
    return BehaviorTreeBuilder()
        .selector()
            .sequence()  // Handle threats
                .leaf(std::make_shared<CheckForThreatsNode>())
                .leaf(std::make_shared<FleeNode>())
            .end()
            .sequence()  // Normal behavior
                .leaf(std::make_shared<CheckResourcesNode>())
                .leaf(std::make_shared<GatherResourcesNode>())
            .end()
        .end()
        .build();
} 