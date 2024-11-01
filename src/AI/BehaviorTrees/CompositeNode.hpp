#pragma once
#include "BehaviorNode.hpp"
#include <vector>

class CompositeNode : public BehaviorNode {
protected:
    std::vector<BehaviorNodePtr> children;

public:
    void add_child(BehaviorNodePtr child) {
        children.push_back(std::move(child));
    }

    const std::vector<BehaviorNodePtr>& get_children() const {
        return children;
    }
}; 