public class IdleState : INPCState
{
    public void Enter(NPCController npc)
    {
        GD.Print($"{npc.Name} is now idling.");
    }

    public void Execute(NPCController npc, float deltaTime)
    {
        // Idle behavior logic
        // Transition conditions can be added here
    }

    public void Exit(NPCController npc)
    {
        GD.Print($"{npc.Name} stops idling.");
    }
}