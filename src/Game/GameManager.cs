// ... existing code ...

public class GameManager : IUpdatable
{
    // ... existing systems ...
    private ReputationSystem reputationSystem;

    public GameManager(List<NPC> npcs, List<Node> nodes, EnvironmentManager environmentManager)
    {
        // ... existing initializations ...
        reputationSystem = new ReputationSystem(this);
        systems.Add(reputationSystem);
                // ... existing initializations ...
        weatherSystem = new WeatherSystem(environmentManager, this);
        systems.Add(weatherSystem);

        // Subscribe AISystem to weather changes
        aiSystem.SubscribeToWeather(environmentManager);
    }

    // ... existing methods ...

    public ReputationSystem GetReputationSystem()
    {
        return reputationSystem;
    }
        // ... existing systems ...
    private WeatherSystem weatherSystem;
}