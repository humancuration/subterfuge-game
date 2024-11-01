#pragma once
#include "BehaviorNode.hpp"
#include "SelectorNode.hpp"
#include "SequenceNode.hpp"
#include <memory>
#include <stack>

class BehaviorTreeBuilder {
private:
    std::stack<std::shared_ptr<CompositeNode>> node_stack;
    BehaviorNodePtr root;

public:
    BehaviorTreeBuilder& selector() {
        auto node = std::make_shared<SelectorNode>();
        push_node(node);
        return *this;
    }

    BehaviorTreeBuilder& sequence() {
        auto node = std::make_shared<SequenceNode>();
        push_node(node);
        return *this;
    }

    BehaviorTreeBuilder& leaf(BehaviorNodePtr node) {
        if (!node_stack.empty()) {
            node_stack.top()->add_child(node);
        } else {
            root = node;
        }
        return *this;
    }

    BehaviorTreeBuilder& end() {
        if (!node_stack.empty()) {
            auto completed_node = node_stack.top();
            node_stack.pop();
            if (node_stack.empty()) {
                root = completed_node;
            } else {
                node_stack.top()->add_child(completed_node);
            }
        }
        return *this;
    }

    BehaviorNodePtr build() {
        while (!node_stack.empty()) {
            end();
        }
        return root;
    }

private:
    void push_node(std::shared_ptr<CompositeNode> node) {
        if (!node_stack.empty()) {
            node_stack.top()->add_child(node);
        }
        node_stack.push(node);
    }
}; 