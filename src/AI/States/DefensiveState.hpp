#pragma once
#include "INPCState.hpp"
#include <godot_cpp/core/class_db.hpp>

class DefensiveState : public INPCState {
public:
    void enter(NPCController* npc) override;
    void execute(NPCController* npc, float deltaTime) override;
    void exit(NPCController* npc) override;
}; 