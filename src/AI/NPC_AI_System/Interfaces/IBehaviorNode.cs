// Interfaces/IBehaviorNode.cs
public interface IBehaviorNode
{
    bool Execute(NPCController npc);
}

// BehaviorTrees/SelectorNode.cs
public class SelectorNode : IBehaviorNode
{
    private List<IBehaviorNode> children;

    public SelectorNode(List<IBehaviorNode> children)
    {
        this.children = children;
    }

    public bool Execute(NPCController npc)
    {
        foreach (var child in children)
        {
            if (child.Execute(npc))
                return true;
        }
        return false;
    }
}