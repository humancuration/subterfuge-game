public class SustainableMining : Technology
{
    public SustainableMining()
    {
        Name = "Sustainable Mining Practices";
        Prerequisites = new List<Technology> { new ElectricVehicles() };
        PositiveEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "Reduced environmental impact of mining", 
                ApplyEffect = () => GameState.Instance.RareEarthDemand -= 5 
            },
            new Effect 
            { 
                Description = "Improved worker conditions", 
                ApplyEffect = () => GameState.Instance.WorkerSatisfaction += 10 
            }
        };
        NegativeEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "Increased mining costs", 
                ApplyEffect = () => GameState.Instance.MiningCosts += 15 
            }
        };
    }
}