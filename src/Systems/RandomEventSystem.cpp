#include "RandomEventSystem.hpp"
#include "../Events/EventManager.hpp"

namespace Systems {

using namespace godot;

void RandomEventSystem::_register_methods() {
    register_method("_init", &RandomEventSystem::_init);
    register_method("update", &RandomEventSystem::update);
}

RandomEventSystem::RandomEventSystem() {}
RandomEventSystem::~RandomEventSystem() {}

void RandomEventSystem::_init() {
    rng.instance();
    rng->randomize();
    initialize_possible_events();
}

void RandomEventSystem::initialize_possible_events() {
    // Create Meteor Shower event
    GameEvent* meteor_shower = GameEvent::_new();
    meteor_shower->set_name("Meteor Shower");
    meteor_shower->set_effect([]() {
        // Implement meteor shower effects on nodes
        Godot::print("Meteor shower occurring!");
    });
    possible_events.push_back(meteor_shower);

    // Create Wandering Musician event
    GameEvent* wandering_musician = GameEvent::_new();
    wandering_musician->set_name("Wandering Musician");
    wandering_musician->set_effect([]() {
        // Implement morale boost effects
        Godot::print("A wandering musician has arrived, boosting morale!");
    });
    possible_events.push_back(wandering_musician);
}

void RandomEventSystem::update(float delta_time) {
    if (should_trigger_event()) {
        GameEvent* event = select_random_event();
        if (event) {
            EventManager::get_instance()->trigger_event(event);
        }
    }
}

bool RandomEventSystem::should_trigger_event() {
    // Example: 0.1% chance per update
    return rng->randf() < 0.001f;
}

GameEvent* RandomEventSystem::select_random_event() {
    if (possible_events.empty()) {
        return nullptr;
    }
    
    int index = rng->randi_range(0, possible_events.size() - 1);
    return possible_events[index];
}

} // namespace Systems 