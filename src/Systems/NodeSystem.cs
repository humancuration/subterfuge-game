// NodeSystem Class

public class NodeSystem : IUpdatable
{
    private List<Node> nodes;

    public NodeSystem(List<Node> nodeList)
    {
        nodes = nodeList;
    }

    public void Update(float deltaTime)
    {
        foreach (var node in nodes)
        {
            node.Update(deltaTime);
        }
    }
}
