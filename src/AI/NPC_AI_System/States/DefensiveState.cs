public class DefensiveState : INPCState
{
    public void Enter(NPCController npc)
    {
        GD.Print($"{npc.Name} is now in Defensive mode due to extreme weather!");
        // Increase defense or take other defensive measures
        npc.Defense += 20f;
    }

    public void Execute(NPCController npc, float deltaTime)
    {
        // Defensive behavior logic during extreme weather
        // Example: Move to designated shelter city
        if (npc.CurrentCity.Name != "Shelter")
        {
            City shelterCity = npc.GameManager.FindShelter();
            npc.MoveToCity(shelterCity);
            GD.Print($"{npc.Name} is seeking shelter...");
        }
    }

    public void Exit(NPCController npc)
    {
        GD.Print($"{npc.Name} exits Defensive mode.");
        npc.Defense -= 20f;
    }
}