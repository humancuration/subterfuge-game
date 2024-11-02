#include "TechnologySystem.h"

#include <Godot.hpp>
#include "Models/GameEvent.h"
#include "EventManager.h"

namespace Systems {

using namespace godot;

void TechnologySystem::_register_methods() {
    register_method("_process", &TechnologySystem::update_system);
}

TechnologySystem::TechnologySystem() {}
TechnologySystem::~TechnologySystem() {}

void TechnologySystem::_init() {
    // Initialization if needed
}

void TechnologySystem::update_system(float delta_time) {
    for (int i = 0; i < nodes.size(); i++) {
        update_technology(i, delta_time);
    }
}

void TechnologySystem::update_technology(int node_index, float delta_time) {
    // Example: Increase Technological Level based on Research Investment
    // Assuming Node has a method get_stats() returning a pointer/reference to stats
    Node *node = Object::cast_to<Node>(nodes[i]);
    if (node) {
        float research_investment = node->get_stats()->research_investment;
        float tech_change = delta_time * research_investment * 0.1f;
        node->get_stats()->technological_level += tech_change;

        // Clamp the value between 0 and 100
        node->get_stats()->technological_level = Math::clamp(node->get_stats()->technological_level, 0.0f, 100.0f);

        // Random technological breakthroughs
        float random_chance = (float)rand() / RAND_MAX;
        if (random_chance < 0.002f) { // 0.2% chance per update
            trigger_tech_breakthrough(i);
        }
    }
}

void TechnologySystem::trigger_tech_breakthrough(int node_index) {
    Node *node = Object::cast_to<Node>(nodes[node_index]);
    if (node) {
        // Create a new GameEvent
        GameEvent renewable_energy;
        renewable_energy.name = "Renewable Energy Breakthrough";
        renewable_energy.effect = [node]() {
            node->get_stats()->environmental_health += 15.0f;
            node->get_stats()->economic_prosperity += 10.0f;
            Godot::print(String("Renewable Energy Breakthrough in ") + node->get_name() + "!");
        };

        // Trigger the event
        EventManager::trigger_event(renewable_energy);
    }
}

} // namespace Systems 