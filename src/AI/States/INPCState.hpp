#pragma once
#include <memory>

class NPCController;

class INPCState {
public:
    virtual ~INPCState() = default;
    virtual void enter(NPCController* npc) = 0;
    virtual void execute(NPCController* npc, float deltaTime) = 0;
    virtual void exit(NPCController* npc) = 0;
}; 