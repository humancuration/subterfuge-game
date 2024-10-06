public class ElectricVehicles : Technology
{
    public ElectricVehicles()
    {
        Name = "Electric Vehicles";
        PositiveEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "Reduced air pollution", 
                ApplyEffect = () => GameState.Instance.AirPollution -= 10 
            },
            new Effect 
            { 
                Description = "Increased energy efficiency", 
                ApplyEffect = () => GameState.Instance.EnergyEfficiency += 5 
            }
        };
        NegativeEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "Increased demand for rare earth minerals", 
                ApplyEffect = () => GameState.Instance.RareEarthDemand += 15 
            },
            new Effect 
            { 
                Description = "Strain on electrical grid", 
                ApplyEffect = () => GameState.Instance.ElectricalGridStrain += 10 
            }
        };
    }
}