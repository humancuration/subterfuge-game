using Godot;
using System;
using System.Collections.Generic;

public class NPC : Entity, IUpdatable
{
    public string Name { get; private set; }
    public Node CurrentNode { get; private set; }
    public Dialogue CurrentDialogue { get; set; } // Add this property

    public NPC(string name, Node startingNode)
    {
        Name = name;
        CurrentNode = startingNode;
    }

    public void Update(float deltaTime)
    {
        // Implement NPC behavior, such as movement, interaction, etc.
    }

    public void MoveToNode(Node node)
    {
        CurrentNode = node;
    }
}