// Interactions/InteractionLayer.cs
public abstract class InteractionLayer
{
    public abstract void InitiateInteraction(NPCController npc, Player player);
}

// Interactions/DialogueInteraction.cs
public class DialogueInteraction : InteractionLayer
{
    public override void InitiateInteraction(NPCController npc, Player player)
    {
        // Handle dialogue logic
        DialogueManager.DisplayDialogue(npc, player);
    }
}