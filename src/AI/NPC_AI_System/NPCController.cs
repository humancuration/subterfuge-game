// ... existing code ...

public class NPCController : Node
{
    public string Name { get; set; }
    public AIController AIController { get; private set; }

    private INPCState currentState;

    public NPCController(string name, AIController aiController)
    {
        Name = name;
        AIController = aiController;
        TransitionToState(new IdleState());
    }

    public void Update(float deltaTime)
    {
        currentState?.Execute(this, deltaTime);
    }

    public void ChangeState(INPCState newState)
    {
        currentState?.Exit(this);
        currentState = newState;
        currentState?.Enter(this);
    }

    // New Method to React to Weather Events
    public void ReactToWeather(WeatherCondition weather)
    {
        switch (weather)
        {
            case WeatherCondition.Storm:
            case WeatherCondition.Snow:
                ChangeState(new DefensiveState());
                break;
            case WeatherCondition.Rain:
                // Maybe seek shelter or adjust morale
                CurrentCity.Stats.Morale -= 2f;
                break;
            case WeatherCondition.Heatwave:
                CurrentCity.Stats.HealthRisk += 5f;
                break;
            // Handle other weather conditions as needed
            default:
                ChangeState(new IdleState());
                break;
        }
    }

    public void TransitionToState(INPCState newState)
    {
        currentState?.Exit(this);
        currentState = newState;
        currentState?.Enter(this);
    }

    // Example method to switch states based on conditions
    public void DecideNextAction()
    {
        // Placeholder for decision logic
        // For instance, switch to PatrolState after idling
        if (currentState is IdleState)
        {
            TransitionToState(new PatrolState());
        }
    }

    public void ReactToEvent(WorldEvent worldEvent)
    {
        // Modify state based on world events
        // Example: If rebellion sparked, switch to a defensive state
        if (worldEvent.EventName == "Rebellion Sparked")
        {
            TransitionToState(new DefensiveState());
        }
    }

    public float CorporateAlignment { get; set; }
    public float PublicAlignment { get; set; }

    public void DecideResearchFocus(ResearchFundingSystem fundingSystem)
    {
        float totalFunding = fundingSystem.PublicFunding + fundingSystem.CorporateFunding;
        float publicRatio = fundingSystem.PublicFunding / totalFunding;
        float corporateRatio = fundingSystem.CorporateFunding / totalFunding;

        // NPC's decision is influenced by both their alignment and the funding ratio
        float publicTendency = (PublicAlignment + publicRatio) / 2;
        float corporateTendency = (CorporateAlignment + corporateRatio) / 2;

        if (publicTendency > corporateTendency)
        {
            // Focus on public-benefit research
            GD.Print($"{Name} focuses on public-benefit research");
            GameState.Instance.PublicResearchProgress += 1;
        }
        else
        {
            // Focus on corporate-benefit research
            GD.Print($"{Name} focuses on corporate-benefit research");
            GameState.Instance.CorporateResearchProgress += 1;
        }
    }
}