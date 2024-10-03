// Traits/BraveryTrait.cs
public class BraveryTrait : ITrait
{
    public string Name => "Bravery";

    public void ApplyEffect(NPCController npc)
    {
        npc.Defense += 20;
        // Additional bravery-related effects
    }
}