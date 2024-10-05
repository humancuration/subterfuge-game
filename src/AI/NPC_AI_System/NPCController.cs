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
                CurrentNode.Stats.Morale -= 2f;
                break;
            case WeatherCondition.Heatwave:
                CurrentNode.Stats.HealthRisk += 5f;
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
}