// EmergentBehavior/EventListener.cs
public class EventListener
{
    public void OnWorldEvent(WorldEvent worldEvent)
    {
        // Notify EmergentBehaviorManager
        EmergentBehaviorManager.Instance.TriggerBehavior(worldEvent);
    }
}

