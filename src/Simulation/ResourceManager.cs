// File: src/Simulation/ResourceManager.cs
using Godot;
using System.Collections.Generic;

public class ResourceManager : Node
{
    public Dictionary<string, int> Resources { get; private set; } = new Dictionary<string, int>();

    public override void _Ready()
    {
        Resources.Add("Gold", 500);
        Resources.Add("Food", 200);
        Resources.Add("Wood", 300);
    }

    public void AddResource(string resource, int amount)
    {
        if (Resources.ContainsKey(resource))
        {
            Resources[resource] += amount;
        }
        else
        {
            Resources.Add(resource, amount);
        }
        GD.Print($"Added {amount} {resource}. Total: {Resources[resource]}");
    }

    public bool ConsumeResource(string resource, int amount)
    {
        if (Resources.ContainsKey(resource) && Resources[resource] >= amount)
        {
            Resources[resource] -= amount;
            GD.Print($"Consumed {amount} {resource}. Remaining: {Resources[resource]}");
            return true;
        }
        GD.Print($"Not enough {resource} to consume.");
        return false;
    }
}