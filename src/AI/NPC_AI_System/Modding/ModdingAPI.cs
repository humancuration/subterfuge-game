// Modding/ModdingAPI.cs
public class ModdingAPI
{
    public void RegisterTrait(ITrait trait)
    {
        TraitsManager.Instance.RegisterTrait(trait);
    }

    public void RegisterInteraction(InteractionLayer interaction)
    {
        InteractionManager.Instance.RegisterInteraction(interaction);
    }

    public void RegisterBehaviorNode(IBehaviorNode node)
    {
        BehaviorTreeManager.Instance.RegisterNode(node);
    }

    // Additional API methods...
}