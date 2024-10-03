// Interfaces/ITrait.cs
public interface ITrait
{
    string Name { get; }
    void ApplyEffect(NPCController npc);
}

// Traits/BraveryTrait.cs
public class BraveryTrait : ITrait
{
    public string Name => "Bravery";

    public void ApplyEffect(NPCController npc)
    {
        npc.Defense += 10;
        // Additional effects...
    }
}