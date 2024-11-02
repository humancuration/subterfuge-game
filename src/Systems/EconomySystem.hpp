#ifndef ECONOMYSYSTEM_HPP
#define ECONOMYSYSTEM_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <vector>
#include "ISystem.hpp"
#include "../Models/Node.hpp"
#include "../Events/GameEvent.hpp"

namespace Systems {

class EconomySystem : public godot::Node, public ISystem {
    GODOT_CLASS(EconomySystem, godot::Node)

private:
    std::vector<Node*> nodes;
    godot::Ref<godot::RandomNumberGenerator> rng;

public:
    static void _register_methods();
    
    EconomySystem();
    ~EconomySystem();

    void _init();
    void set_nodes(const std::vector<Node*>& node_list);
    void update(float delta_time) override;

private:
    void update_economy(Node* node, float delta_time);
    void trigger_economic_event(Node* node);
};

} // namespace Systems

#endif // ECONOMYSYSTEM_HPP 