public class ArtificialIntelligence : Technology
{
    public ArtificialIntelligence()
    {
        Name = "Artificial Intelligence";
        BasePublicBenefit = 50;
        BaseCorporateBenefit = 100;

        PositiveEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "Increased productivity", 
                ApplyEffect = () => GameState.Instance.EconomicProductivity += 20 
            },
            new Effect 
            { 
                Description = "Advanced medical diagnostics", 
                ApplyEffect = () => GameState.Instance.PublicHealth += 10 
            }
        };
        NegativeEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "Job displacement", 
                ApplyEffect = () => GameState.Instance.Unemployment += 5 
            },
            new Effect 
            { 
                Description = "Increased wealth inequality", 
                ApplyEffect = () => GameState.Instance.WealthInequality += 10 
            }
        };
    }
}