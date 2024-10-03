using Godot;
using System;
using System.Collections.Generic;

namespace Models
{
    public class Character
    {
        public string Name { get; set; }
        public string Appearance { get; set; } // Could be a string representing appearance options
        public Dictionary<string, int> Skills { get; set; } = new Dictionary<string, int>();
        public List<string> Traits { get; set; } = new List<string>();

        public Character(string name, string appearance)
        {
            Name = name;
            Appearance = appearance;
        }

        public void AddSkill(string skillName, int skillLevel)
        {
            Skills[skillName] = skillLevel;
        }

        public void AddTrait(string traitName)
        {
            Traits.Add(traitName);
        }
    }
}