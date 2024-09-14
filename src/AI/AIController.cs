// File: src/AI/AIController.cs
using Godot;
using System.Collections.Generic;

public class AIController : Node
{
    private Node currentNode;
    private List<Node> visitedNodes = new List<Node>();

    public void SetStartNode(Node startNode)
    {
        currentNode = startNode;
        visitedNodes.Add(currentNode);
    }

    public void MoveToNextNode()
    {
        if (currentNode.ConnectedNodes.Count == 0)
            return;

        // Simple random movement
        Random rnd = new Random();
        Node nextNode = currentNode.ConnectedNodes[rnd.Next(currentNode.ConnectedNodes.Count)];
        currentNode = nextNode;

        if (!visitedNodes.Contains(currentNode))
            visitedNodes.Add(currentNode);

        GD.Print($"AI moved to {currentNode.NodeName}");
    }
}