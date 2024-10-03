// Interactions/TradeInteraction.cs
public class TradeInteraction : InteractionLayer
{
    public override void InitiateInteraction(NPCController npc, Player player)
    {
        // Handle trading logic
        TradeManager.OpenTradeUI(npc, player);
    }
}