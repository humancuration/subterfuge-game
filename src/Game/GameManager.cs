// ... existing code ...

public class GameManager : IUpdatable
{
    // ... existing systems ...
    private ReputationSystem reputationSystem;

    public GameManager(List<NPC> npcs, List<Node> nodes)
    {
        // ... existing initializations ...
        reputationSystem = new ReputationSystem(this);
        systems.Add(reputationSystem);
    }

    // ... existing methods ...

    public ReputationSystem GetReputationSystem()
    {
        return reputationSystem;
    }
}