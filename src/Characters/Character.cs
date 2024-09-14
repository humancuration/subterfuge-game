// File: src/Characters/Character.cs
using Godot;
using System.Collections.Generic;

public class Character : Node
{
    public string Name { get; set; }
    public int Level { get; private set; }
    public Dictionary<string, int> Attributes { get; private set; } = new Dictionary<string, int>();
    public List<string> Skills { get; private set; } = new List<string>();

    public Character(string name)
    {
        Name = name;
        Level = 1;
        Attributes.Add("Strength", 10);
        Attributes.Add("Agility", 10);
        Attributes.Add("Intelligence", 10);
    }

    public void LevelUp()
    {
        Level++;
        // Increase attributes on level up
        Attributes["Strength"] += 2;
        Attributes["Agility"] += 2;
        Attributes["Intelligence"] += 2;
        GD.Print($"{Name} leveled up to Level {Level}!");
    }

    public void LearnSkill(string skillName)
    {
        if (!Skills.Contains(skillName))
        {
            Skills.Add(skillName);
            GD.Print($"{Name} learned new skill: {skillName}");
        }
    }
}