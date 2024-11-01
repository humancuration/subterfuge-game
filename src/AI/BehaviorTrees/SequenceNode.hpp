#pragma once
#include "CompositeNode.hpp"

class SequenceNode : public CompositeNode {
private:
    size_t current_child{0};

public:
    Status execute(NPCController* npc) override {
        while (current_child < children.size()) {
            Status status = children[current_child]->execute(npc);
            
            if (status != Status::SUCCESS) {
                if (status == Status::RUNNING) {
                    return Status::RUNNING;
                }
                current_child = 0;
                return Status::FAILURE;
            }
            ++current_child;
        }
        
        current_child = 0;
        return Status::SUCCESS;
    }
}; 