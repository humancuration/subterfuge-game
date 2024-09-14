// 6. Additional Systems and Enhancements
// To further enrich your game, consider implementing the following systems:
// 6.1. Technology System
// Manages technological advancements within each node, influencing economic prosperity and environmental health.
// File: Systems/TechnologySystem.cs

using System;
using System.Collections.Generic;
using Models;

namespace Systems
{
    public class TechnologySystem : IUpdatable
    {
        private List<Node> nodes;
        private Random rand;

        public TechnologySystem(List<Node> nodeList)
        {
            nodes = nodeList;
            rand = new Random();
        }

        public void Update(float deltaTime)
        {
            foreach (var node in nodes)
            {
                UpdateTechnology(node, deltaTime);
            }
        }

        private void UpdateTechnology(Node node, float deltaTime)
        {
            // Example: Increase Technological Level based on Research Investment
            float techChange = deltaTime * node.Stats.ResearchInvestment * 0.1f;
            node.Stats.TechnologicalLevel += techChange;

            // Ensure stats remain within logical bounds
            node.Stats.TechnologicalLevel = Math.Clamp(node.Stats.TechnologicalLevel, 0f, 100f);

            // Random technological breakthroughs
            if (rand.NextDouble() < 0.002) // 0.2% chance per update
            {
                TriggerTechBreakthrough(node);
            }
        }

        private void TriggerTechBreakthrough(Node node)
        {
            // Example tech event: Renewable Energy Adoption
            GameEvent renewableEnergy = new GameEvent("Renewable Energy Breakthrough", () =>
            {
                node.Stats.EnvironmentalHealth += 15f;
                node.Stats.EconomicProsperity += 10f;
                Console.WriteLine($"Renewable Energy Breakthrough in {node.Name}! Environmental Health and Economic Prosperity increased.");
            });

            EventManager.TriggerEvent(renewableEnergy);
        }
    }
}

// Explanation:
// TechnologySystem Class: Tracks and updates the technological level of each node based on research investments. It can also trigger technological breakthroughs that positively impact the environment and economy.