// EmergentBehavior/EmergentBehaviorManager.cs
public class EmergentBehaviorManager
{
    private static EmergentBehaviorManager instance;
    public static EmergentBehaviorManager Instance => instance ?? (instance = new EmergentBehaviorManager());

    public void TriggerBehavior(WorldEvent worldEvent)
    {
        // Define how NPCs react to the event
        foreach (var npc in NPCManager.Instance.GetAllNPCs())
        {
            npc.ReactToEvent(worldEvent);
        }
    }
}