// File: src/Buildings/Building.cs
using Godot;

public abstract class Building : Node
{
    public string BuildingName { get; set; }
    public int Cost { get; set; }

    public abstract void Activate(Node ownerNode);
}

// File: src/Buildings/Farm.cs
public class Farm : Building
{
    public Farm()
    {
        BuildingName = "Farm";
        Cost = 100;
    }

    public override void Activate(Node ownerNode)
    {
        // Example: Increase food resources
        var gameState = ownerNode.GetNode<GameState>("../GameState");
        gameState.Food += 10;
        GD.Print("Farm activated: +10 Food");
    }
}