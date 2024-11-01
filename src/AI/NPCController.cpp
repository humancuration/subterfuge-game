#include "NPCController.hpp"
#include "States/DefensiveState.hpp"
#include "States/IdleState.hpp"
#include <godot_cpp/core/class_db.hpp>

void NPCController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update", "delta"), &NPCController::update);
    ClassDB::bind_method(D_METHOD("get_name"), &NPCController::get_name);
    ClassDB::bind_method(D_METHOD("get_defense"), &NPCController::get_defense);
    ClassDB::bind_method(D_METHOD("set_defense", "value"), &NPCController::set_defense);
}

NPCController::NPCController() = default;

NPCController::NPCController(std::string npcName, std::unique_ptr<AIController> controller)
    : name(std::move(npcName))
    , aiController(std::move(controller)) {
    currentState = std::make_unique<IdleState>();
}

void NPCController::update(float deltaTime) {
    if (currentState) {
        currentState->execute(this, deltaTime);
    }
}

void NPCController::change_state(std::unique_ptr<INPCState> newState) {
    if (currentState) {
        currentState->exit(this);
    }
    currentState = std::move(newState);
    if (currentState) {
        currentState->enter(this);
    }
}

void NPCController::react_to_weather(WeatherCondition weather) {
    switch (weather) {
        case WeatherCondition::Storm:
        case WeatherCondition::Snow:
            change_state(std::make_unique<DefensiveState>());
            break;
        default:
            change_state(std::make_unique<IdleState>());
            break;
    }
}

void NPCController::decide_next_action() {
    // Implement decision-making logic
    // This could use behavior trees or other AI decision systems
} 