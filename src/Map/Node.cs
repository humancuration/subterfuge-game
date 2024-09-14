using Godot;
using System;
using System.Collections.Generic;

public class Node : Node2D, IUpdatable
{
    // Core properties
    public int Id { get; private set; }
    public string NodeName { get; set; }
    public NodeStats Stats { get; private set; }
    public List<Node> ConnectedNodes { get; private set; } = new List<Node>();
    public bool IsVisited { get; private set; } = false;
    public int UpgradeLevel { get; private set; } = 0;

    // Events
    public event Action<Node> OnVisited;
    public event Action<Node, int> OnUpgraded;
    public event Action<Node, Node> OnNodeConnected;

    public Node(int id, string name)
    {
        Id = id;
        NodeName = name;
        Stats = new NodeStats();
    }

    public void ConnectNode(Node node)
    {
        if (!ConnectedNodes.Contains(node))
        {
            ConnectedNodes.Add(node);
            node.ConnectNode(this); // Ensure bi-directional connection
            OnNodeConnected?.Invoke(this, node);
            GD.Print($"{NodeName} connected to {node.NodeName}");
        }
    }

    public void Visit()
    {
        if (!IsVisited)
        {
            IsVisited = true;
            OnVisited?.Invoke(this);
            GD.Print($"Visited {NodeName}");
            // Trigger events or unlock paths
        }
    }

    public void Upgrade()
    {
        UpgradeLevel++;
        OnUpgraded?.Invoke(this, UpgradeLevel);
        GD.Print($"{NodeName} upgraded to level {UpgradeLevel}");
        ApplyUpgradeBenefits();
    }

    private void ApplyUpgradeBenefits()
    {
        // Apply upgrade benefits to Stats
        Stats.ResourceAvailability += 5f;
        Stats.TechnologicalLevel += 3f;
        Stats.Infrastructure += 2f;
        // Add more stat improvements as needed
    }

    public void Update(float deltaTime)
    {
        // Update stats based on modifiers
        Stats.Update(deltaTime);

        // Additional update logic for the node
        UpdateNodeLogic(deltaTime);
    }

    private void UpdateNodeLogic(float deltaTime)
    {
        // Implement additional node-specific logic here
        // For example, handle resource production, population growth, etc.
    }

    public override string ToString()
    {
        return $"Node {Id}: {NodeName} (Level {UpgradeLevel})";
    }
}

// Here are the improvements and additions made to the Node class:
// Inherited from both Node2D (Godot) and implemented IUpdatable interface for consistency with the game system.
// Added an Id property for unique identification.
// Kept the NodeStats from the second version for detailed stat tracking.
// Maintained the IsVisited and UpgradeLevel properties from the first version.
// Added events (OnVisited, OnUpgraded, OnNodeConnected) to allow other parts of the game to react to node changes.
// Improved the ConnectNode method to ensure bi-directional connections and trigger an event.
// Enhanced the Visit method to only mark as visited once and trigger an event.
// Expanded the Upgrade method to trigger an event and call ApplyUpgradeBenefits.
// Added an ApplyUpgradeBenefits method to improve node stats when upgraded.
// Implemented the Update method from IUpdatable to update stats and added a placeholder for additional node-specific logic.
// 11. Added a ToString method for easier debugging and logging.
// This enhanced Node class combines the Godot integration from the first version with the more detailed properties and methods from the second version. It also adds new features like events and upgrade benefits, making it more flexible and powerful for your game's needs.
// Remember to adjust the namespaces and using statements as needed to fit your project structure. Also, ensure that the NodeStats class is properly implemented with all the stats mentioned in the ApplyUpgradeBenefits method.