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

    public void UpgradeBuilding(Node node, Building building)
    {
        if (building.CanUpgrade() && resourceManager.ConsumeResource("Metal", building.UpgradeCost))
        {
            building.Upgrade();
            GD.Print($"Upgraded {building.BuildingName} on {node.NodeName} to level {building.Level}");
        }
        else
        {
            GD.Print("Cannot upgrade building. Check resources or max level.");
        }
    }

    public void DestroyBuilding(Node node, Building building)
    {
        node.RemoveChild(building);
        building.Deactivate();
        GD.Print($"Destroyed {building.BuildingName} on {node.NodeName}");
    }

    public bool CanBuildOnNode(Node node, Building building)
    {
        return node.AvailableSpace >= building.SpaceRequired && resourceManager.HasResource("Wood", building.Cost);
    }

    public void ProcessTurn()
    {
        foreach (var node in dataManager.GetAllNodes())
        {
            foreach (var building in node.GetChildren().OfType<Building>())
            {
                building.ProcessTurn();
            }
            resourceManager.CollectResources(node);
        }
        eventManager.ProcessEvents();
        GD.Print("Turn processed.");
    }
}

// File: tests/Game/GameControllerTests.cs
using NUnit.Framework;
using Moq;

[TestFixture]
public class GameControllerTests
{
    private GameController gameController;
    private Mock<ModManager> mockModManager;
    private Mock<EventManager> mockEventManager;
    private Mock<ResourceManager> mockResourceManager;
    private Mock<DataManager> mockDataManager;

    [SetUp]
    public void Setup()
    {
        gameController = new GameController();
        mockModManager = new Mock<ModManager>();
        mockEventManager = new Mock<EventManager>();
        mockResourceManager = new Mock<ResourceManager>();
        mockDataManager = new Mock<DataManager>();

        // Use reflection to set private fields
        typeof(GameController).GetField("modManager", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance).SetValue(gameController, mockModManager.Object);
        typeof(GameController).GetField("eventManager", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance).SetValue(gameController, mockEventManager.Object);
        typeof(GameController).GetField("resourceManager", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance).SetValue(gameController, mockResourceManager.Object);
        typeof(GameController).GetField("dataManager", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance).SetValue(gameController, mockDataManager.Object);
    }

    [Test]
    public void OnNodeVisited_ShouldTriggerRandomEvent()
    {
        var mockNode = new Mock<Node>();
        mockNode.Setup(n => n.NodeName).Returns("TestNode");

        gameController.OnNodeVisited(mockNode.Object);

        mockEventManager.Verify(em => em.TriggerRandomEvent(), Times.Once);
    }

    [Test]
    public void BuildOnNode_ShouldAddBuildingWhenEnoughResources()
    {
        var mockNode = new Mock<Node>();
        var mockBuilding = new Mock<Building>();
        mockBuilding.Setup(b => b.Cost).Returns(10);
        mockBuilding.Setup(b => b.BuildingName).Returns("TestBuilding");
        mockNode.Setup(n => n.NodeName).Returns("TestNode");

        mockResourceManager.Setup(rm => rm.ConsumeResource("Wood", 10)).Returns(true);

        gameController.BuildOnNode(mockNode.Object, mockBuilding.Object);

        mockNode.Verify(n => n.AddChild(mockBuilding.Object), Times.Once);
        mockBuilding.Verify(b => b.Activate(mockNode.Object), Times.Once);
    }

    [Test]
    public void BuildOnNode_ShouldNotBuildWhenNotEnoughResources()
    {
        var mockNode = new Mock<Node>();
        var mockBuilding = new Mock<Building>();
        mockBuilding.Setup(b => b.Cost).Returns(10);
        mockResourceManager.Setup(rm => rm.ConsumeResource("Wood", 10)).Returns(false);

        gameController.BuildOnNode(mockNode.Object, mockBuilding.Object);

        mockNode.Verify(n => n.AddChild(It.IsAny<Building>()), Times.Never);
        mockBuilding.Verify(b => b.Activate(It.IsAny<Node>()), Times.Never);
    }

    [Test]
    public void UpgradeBuilding_ShouldUpgradeWhenPossible()
    {
        var mockNode = new Mock<Node>();
        var mockBuilding = new Mock<Building>();
        mockBuilding.Setup(b => b.CanUpgrade()).Returns(true);
        mockBuilding.Setup(b => b.UpgradeCost).Returns(5);
        mockBuilding.Setup(b => b.BuildingName).Returns("TestBuilding");
        mockBuilding.Setup(b => b.Level).Returns(2);
        mockNode.Setup(n => n.NodeName).Returns("TestNode");

        mockResourceManager.Setup(rm => rm.ConsumeResource("Metal", 5)).Returns(true);

        gameController.UpgradeBuilding(mockNode.Object, mockBuilding.Object);

        mockBuilding.Verify(b => b.Upgrade(), Times.Once);
    }

    [Test]
    public void UpgradeBuilding_ShouldNotUpgradeWhenNotPossible()
    {
        var mockNode = new Mock<Node>();
        var mockBuilding = new Mock<Building>();
        mockBuilding.Setup(b => b.CanUpgrade()).Returns(false);

        gameController.UpgradeBuilding(mockNode.Object, mockBuilding.Object);

        mockBuilding.Verify(b => b.Upgrade(), Times.Never);
    }

    [Test]
    public void DestroyBuilding_ShouldRemoveAndDeactivateBuilding()
    {
        var mockNode = new Mock<Node>();
        var mockBuilding = new Mock<Building>();
        mockBuilding.Setup(b => b.BuildingName).Returns("TestBuilding");
        mockNode.Setup(n => n.NodeName).Returns("TestNode");

        gameController.DestroyBuilding(mockNode.Object, mockBuilding.Object);

        mockNode.Verify(n => n.RemoveChild(mockBuilding.Object), Times.Once);
        mockBuilding.Verify(b => b.Deactivate(), Times.Once);
    }

    [Test]
    public void CanBuildOnNode_ShouldReturnTrueWhenConditionsMet()
    {
        var mockNode = new Mock<Node>();
        var mockBuilding = new Mock<Building>();
        mockNode.Setup(n => n.AvailableSpace).Returns(10);
        mockBuilding.Setup(b => b.SpaceRequired).Returns(5);
        mockBuilding.Setup(b => b.Cost).Returns(20);

        mockResourceManager.Setup(rm => rm.HasResource("Wood", 20)).Returns(true);

        bool result = gameController.CanBuildOnNode(mockNode.Object, mockBuilding.Object);

        Assert.IsTrue(result);
    }

    [Test]
    public void CanBuildOnNode_ShouldReturnFalseWhenInsufficientSpace()
    {
        var mockNode = new Mock<Node>();
        var mockBuilding = new Mock<Building>();
        mockNode.Setup(n => n.AvailableSpace).Returns(5);
        mockBuilding.Setup(b => b.SpaceRequired).Returns(10);
        mockBuilding.Setup(b => b.Cost).Returns(20);

        mockResourceManager.Setup(rm => rm.HasResource("Wood", 20)).Returns(true);

        bool result = gameController.CanBuildOnNode(mockNode.Object, mockBuilding.Object);

        Assert.IsFalse(result);
    }

    [Test]
    public void ProcessTurn_ShouldProcessAllNodesAndBuildings()
    {
        var mockNode1 = new Mock<Node>();
        var mockNode2 = new Mock<Node>();
        var mockBuilding1 = new Mock<Building>();
        var mockBuilding2 = new Mock<Building>();

        mockNode1.Setup(n => n.GetChildren()).Returns(new Godot.Collections.Array { mockBuilding1.Object });
        mockNode2.Setup(n => n.GetChildren()).Returns(new Godot.Collections.Array { mockBuilding2.Object });

        mockDataManager.Setup(dm => dm.GetAllNodes()).Returns(new List<Node> { mockNode1.Object, mockNode2.Object });

        gameController.ProcessTurn();

        mockBuilding1.Verify(b => b.ProcessTurn(), Times.Once);
        mockBuilding2.Verify(b => b.ProcessTurn(), Times.Once);
        mockResourceManager.Verify(rm => rm.CollectResources(It.IsAny<Node>()), Times.Exactly(2));
        mockEventManager.Verify(em => em.ProcessEvents(), Times.Once);
    }
}
