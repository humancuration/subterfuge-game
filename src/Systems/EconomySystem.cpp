#include "EconomySystem.hpp"
#include "../Events/EventManager.hpp"
#include <cmath>

namespace Systems {

using namespace godot;

void EconomySystem::_register_methods() {
    register_method("_init", &EconomySystem::_init);
    register_method("update", &EconomySystem::update);
}

EconomySystem::EconomySystem() {}
EconomySystem::~EconomySystem() {}

void EconomySystem::_init() {
    rng.instance();
    rng->randomize();
}

void EconomySystem::set_nodes(const std::vector<Node*>& node_list) {
    nodes = node_list;
}

void EconomySystem::update(float delta_time) {
    for (auto* node : nodes) {
        update_economy(node, delta_time);
    }
}

void EconomySystem::update_economy(Node* node, float delta_time) {
    // Update Economic Prosperity based on Resource Availability and Population Density
    float prosperity_change = delta_time * (
        node->get_stats().resource_availability * 0.1f - 
        node->get_stats().population_density * 0.05f
    );
    
    node->modify_economic_prosperity(prosperity_change);

    // Random economic events
    if (rng->randf() < 0.005f) { // 0.5% chance per update
        trigger_economic_event(node);
    }
}

void EconomySystem::trigger_economic_event(Node* node) {
    // Create Market Boom event
    GameEvent* market_boom = GameEvent::_new();
    market_boom->set_name("Market Boom");
    
    // Create lambda for event effects
    auto effect = [node]() {
        node->modify_economic_prosperity(20.0f);
        node->modify_resource_availability(10.0f);
        Godot::print(String("Market Boom in {0}! Economic Prosperity and Resources increased.")
            .format(Array::make(node->get_name())));
    };
    
    market_boom->set_effect(effect);

    // Trigger the event through EventManager
    EventManager::get_instance()->trigger_event(market_boom);
}

} // namespace Systems 