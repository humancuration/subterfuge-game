using Godot;
using System;
using System.Collections.Generic;

public class PopulationSystem : Node, IUpdatable
{
    private GameManager gameManager;
    private Random rnd = new Random();

    // Prefabs (PackedScene instances)
    private PackedScene treePrefab;
    private PackedScene buildingPrefab;

    public PopulationSystem(GameManager manager)
    {
        gameManager = manager;
    }

    public override void _Ready()
    {
        treePrefab = GD.Load<PackedScene>("res://src/Graphics/Models/Trees/Tree.tscn");
        buildingPrefab = GD.Load<PackedScene>("res://src/Graphics/Models/Buildings/Building.tscn");
    }

    public void Update(float deltaTime)
    {
        // Example: Populate areas periodically
        // You can tie this to game time or specific events

        // For demonstration, let's populate every 60 seconds
        // Implement a timer or counter as needed
    }

    public void PopulateArea(Vector3 position, string type)
    {
        if (type == "tree")
        {
            var tree = (MeshInstance)treePrefab.Instance();
            tree.Transform = new Transform(Basis.Identity, position + new Vector3(rnd.Next(-5, 5), 0, rnd.Next(-5, 5)));
            AddChild(tree);
        }
        else if (type == "building")
        {
            var building = (MeshInstance)buildingPrefab.Instance();
            building.Transform = new Transform(Basis.Identity, position + new Vector3(rnd.Next(-10, 10), 0, rnd.Next(-10, 10)));
            AddChild(building);
        }
    }

    // Example method to populate multiple areas
    public void PopulateMultipleAreas()
    {
        foreach (var node in gameManager.Nodes)
        {
            // Populate trees around each node
            for (int i = 0; i < 10; i++)
            {
                PopulateArea(node.Position, "tree");
            }

            // Populate buildings around each node
            for (int i = 0; i < 5; i++)
            {
                PopulateArea(node.Position, "building");
            }
        }
    }
}