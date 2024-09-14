// File: src/Map/MapGenerator.cs
using Godot;
using System;
using System.Collections.Generic;

public class MapGenerator : Node
{
    private Random rnd = new Random();

    public List<Node> GenerateMap(int numberOfNodes)
    {
        List<Node> mapNodes = new List<Node>();
        for (int i = 0; i < numberOfNodes; i++)
        {
            Node newNode = new Node
            {
                NodeName = $"Node_{i}",
                Position = new Vector2(rnd.Next(-500, 500), rnd.Next(-500, 500))
            };
            mapNodes.Add(newNode);
            AddChild(newNode);
        }

        // Connect nodes randomly
        foreach (var node in mapNodes)
        {
            int connections = rnd.Next(1, 4);
            for (int j = 0; j < connections; j++)
            {
                Node target = mapNodes[rnd.Next(mapNodes.Count)];
                if (target != node)
                {
                    node.ConnectNode(target);
                }
            }
        }

        GD.Print("Map generation complete.");
        return mapNodes;
    }
}