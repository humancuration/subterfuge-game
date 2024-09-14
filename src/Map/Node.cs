// File: src/Map/Node.cs
using Godot;
using System.Collections.Generic;

public class Node : Node2D
{
    public string NodeName { get; set; }
    public List<Node> ConnectedNodes { get; private set; } = new List<Node>();
    public bool IsVisited { get; private set; } = false;
    public int UpgradeLevel { get; private set; } = 0;

    public void ConnectNode(Node node)
    {
        if (!ConnectedNodes.Contains(node))
        {
            ConnectedNodes.Add(node);
            GD.Print($"{NodeName} connected to {node.NodeName}");
        }
    }

    public void Visit()
    {
        IsVisited = true;
        GD.Print($"Visited {NodeName}");
        // Trigger events or unlock paths
    }

    public void Upgrade()
    {
        UpgradeLevel++;
        GD.Print($"{NodeName} upgraded to level {UpgradeLevel}");
        // Apply upgrade benefits
    }
}