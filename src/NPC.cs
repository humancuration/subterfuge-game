// 3. NPCs and Traits
// NPC Class

public class NPC : Entity, IUpdatable
{
    // Reference to the node they are currently in
    public Node CurrentNode { get; private set; }

    // Traits
    public List<ITrait> Traits { get; private set; }

    // Constructors
    public NPC(int id, string name, Node startingNode) : base(id)
    {
        Name = name;
        CurrentNode = startingNode;
        Traits = new List<ITrait>();
    }

    // Update method for processing NPC logic
    public void Update(float deltaTime)
    {
        // Update traits
        foreach (var trait in Traits)
        {
            trait.Update(this, deltaTime);
        }

        // Additional NPC logic
    }

    // Methods to move between nodes
    public void MoveToNode(Node node)
    {
        CurrentNode = node;
    }
}
