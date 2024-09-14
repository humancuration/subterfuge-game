// Trait Interface and Examples

public interface ITrait
{
    string TraitName { get; }
    void Update(NPC npc, float deltaTime);
}

public class OptimistTrait : ITrait
{
    public string TraitName => "Optimist";

    public void Update(NPC npc, float deltaTime)
    {
        // Increase morale over time
        npc.CurrentNode.Stats.Morale += deltaTime * 0.1f;
    }
}

public class SaboteurTrait : ITrait
{
    public string TraitName => "Saboteur";

    public void Update(NPC npc, float deltaTime)
    {
        // Decrease productivity
        npc.CurrentNode.Stats.EconomicProsperity -= deltaTime * 0.2f;
    }
}
