// File: src/Game/GameController.cs
using Godot;

public class GameController : Node
{
    private ModManager modManager;
    private EventManager eventManager;
    private ResourceManager resourceManager;
    private DataManager dataManager;

    public override void _Ready()
    {
        modManager = GetNode<ModManager>("ModManager");
        eventManager = GetNode<EventManager>("EventManager");
        resourceManager = GetNode<ResourceManager>("ResourceManager");
        dataManager = GetNode<DataManager>("DataManager");

        modManager.InitializeMods();
    }

    public void OnNodeVisited(Node node)
    {
        GD.Print($"Node {node.NodeName} visited.");
        eventManager.TriggerRandomEvent();
        // Additional logic like unlocking paths or upgrades
    }

    public void BuildOnNode(Node node, Building building)
    {
        if (resourceManager.ConsumeResource("Wood", building.Cost))
        {
            node.AddChild(building);
            building.Activate(node);
            GD.Print($"Built {building.BuildingName} on {node.NodeName}");
        }
        else
        {
            GD.Print("Not enough resources to build.");
        }
    }
}