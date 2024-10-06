public class PublicTransportNetwork : Technology
{
    public PublicTransportNetwork()
    {
        Name = "Comprehensive Public Transport Network";
        PositiveEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "Reduced overall traffic", 
                ApplyEffect = () => 
                {
                    GameState.Instance.TrafficCongestion -= 20;
                    GameState.Instance.AirPollution -= 15;
                }
            },
            new Effect 
            { 
                Description = "Increased city accessibility", 
                ApplyEffect = () => GameState.Instance.CityAccessibility += 25 
            }
        };
        NegativeEffects = new List<Effect>
        {
            new Effect 
            { 
                Description = "High initial infrastructure cost", 
                ApplyEffect = () => GameState.Instance.CityBudget -= 1000000 
            },
            new Effect 
            { 
                Description = "Longer individual travel times", 
                ApplyEffect = () => GameState.Instance.AverageCommuteTime += 10 
            }
        };
    }
}