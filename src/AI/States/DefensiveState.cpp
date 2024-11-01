#include "DefensiveState.hpp"
#include "../NPCController.hpp"
#include <godot_cpp/core/class_db.hpp>

void DefensiveState::enter(NPCController* npc) {
    godot::UtilityFunctions::print(godot::String("{0} is now in Defensive mode!").format(Array::make(npc->get_name())));
    npc->set_defense(npc->get_defense() + 20.0f);
}

void DefensiveState::execute(NPCController* npc, float deltaTime) {
    // Implement defensive behavior
    // Example: Check for threats, move to safe locations, etc.
}

void DefensiveState::exit(NPCController* npc) {
    godot::UtilityFunctions::print(godot::String("{0} exits Defensive mode.").format(Array::make(npc->get_name())));
    npc->set_defense(npc->get_defense() - 20.0f);
} 