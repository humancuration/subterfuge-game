// File: src/Skills/Skill.cs
using Godot;

public abstract class Skill : Node
{
    public string SkillName { get; set; }
    public int ManaCost { get; set; }

    public abstract void Use(Character character);
}

// File: src/Skills/Fireball.cs
public class Fireball : Skill
{
    public Fireball()
    {
        SkillName = "Fireball";
        ManaCost = 20;
    }

    public override void Use(Character character)
    {
        // Implement fireball effect
        GD.Print($"{character.Name} casts Fireball!");
        // Reduce mana, deal damage, etc.
    }
}