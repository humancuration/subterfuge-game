// Mods/CustomTraits.cs
public class CustomTraitsMod
{
    public void Initialize(ModdingAPI api)
    {
        ITrait generosity = new GenerosityTrait();
        api.RegisterTrait(generosity);
    }
}

public class GenerosityTrait : ITrait
{
    public string Name => "Generosity";

    public void ApplyEffect(NPCController npc)
    {
        npc.TradeDiscount += 10;
        // Additional generosity-related effects
    }
}