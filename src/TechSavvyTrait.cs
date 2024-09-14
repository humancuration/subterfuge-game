// 6.3. Implementing Technology Traits
// Define traits that influence or are influenced by technological levels.
// File: Models/Traits/TechSavvyTrait.cs

using System;

namespace Models
{
    public class TechSavvyTrait : ITrait
    {
        public string TraitName => "Tech Savvy";

        public void Update(NPC npc, float deltaTime)
        {
            // Increase the node's technological level faster
            npc.CurrentNode.Stats.ResearchInvestment += deltaTime * 0.2f;
        }
    }
}

// Explanation:
// TechSavvyTrait Class: An NPC with this trait contributes more to the node's research investments, accelerating technological advancements.