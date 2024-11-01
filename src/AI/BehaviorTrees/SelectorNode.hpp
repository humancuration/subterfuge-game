#pragma once
#include "CompositeNode.hpp"

class SelectorNode : public CompositeNode {
public:
    Status execute(NPCController* npc) override {
        for (const auto& child : children) {
            Status status = child->execute(npc);
            if (status != Status::FAILURE) {
                return status;
            }
        }
        return Status::FAILURE;
    }
}; 