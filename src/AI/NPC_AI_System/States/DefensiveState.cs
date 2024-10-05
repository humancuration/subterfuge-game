public class DefensiveState : INPCState
{
    public void Enter(NPCController npc)
    {
        GD.Print($"{npc.Name} is now in Defensive mode!");
        // Increase defense or take other defensive measures
    }

    public void Execute(NPCController npc, float deltaTime)
    {
        // Defensive behavior logic
        // For example, seek out dangerous areas or protect certain nodes
    }

    public void Exit(NPCController npc)
    {
        GD.Print($"{npc.Name} exits Defensive mode.");
    }
}