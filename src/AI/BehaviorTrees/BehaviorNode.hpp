#pragma once
#include <memory>
#include <vector>
#include "../NPCController.hpp"

class BehaviorNode {
public:
    enum class Status {
        SUCCESS,
        FAILURE,
        RUNNING
    };

    virtual ~BehaviorNode() = default;
    virtual Status execute(NPCController* npc) = 0;
};

using BehaviorNodePtr = std::shared_ptr<BehaviorNode>; 