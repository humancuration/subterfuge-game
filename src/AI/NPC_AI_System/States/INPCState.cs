public interface INPCState
{
    void Enter(NPCController npc);
    void Execute(NPCController npc, float deltaTime);
    void Exit(NPCController npc);
}