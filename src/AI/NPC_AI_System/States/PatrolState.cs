public class PatrolState : INPCState
{
    public void Enter(NPCController npc)
    {
        GD.Print($"{npc.Name} has started patrolling.");
    }

    public void Execute(NPCController npc, float deltaTime)
    {
        // Patrol behavior logic
        // For example, move to the next waypoint
        npc.AIController.MoveToNextWaypoint();
    }

    public void Exit(NPCController npc)
    {
        GD.Print($"{npc.Name} stops patrolling.");
    }
}