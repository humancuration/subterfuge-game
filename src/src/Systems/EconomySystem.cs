// 2. Economy System
// The Economy System manages resources, production, and economic prosperity within each node.
// 2.1. Economy System Class
// File: Models/EconomySystem.cs

using System;
using System.Collections.Generic;
using Models;

namespace Systems
{
    public class EconomySystem : IUpdatable
    {
        private List<Node> nodes;
        private Random rand;

        public EconomySystem(List<Node> nodeList)
        {
            nodes = nodeList;
            rand = new Random();
        }

        public void Update(float deltaTime)
        {
            foreach (var node in nodes)
            {
                UpdateEconomy(node, deltaTime);
            }
        }

        private void UpdateEconomy(Node node, float deltaTime)
        {
            // Example: Increase Economic Prosperity based on Resource Availability and Population Density
            float prosperityChange = deltaTime * (node.Stats.ResourceAvailability * 0.1f - node.Stats.PopulationDensity * 0.05f);
            node.Stats.EconomicProsperity += prosperityChange;

            // Ensure stats remain within logical bounds
            node.Stats.EconomicProsperity = Math.Clamp(node.Stats.EconomicProsperity, 0f, 100f);

            // Random economic events
            if (rand.NextDouble() < 0.005) // 0.5% chance per update
            {
                TriggerEconomicEvent(node);
            }
        }

        private void TriggerEconomicEvent(Node node)
        {
            // Example economic event: Market Boom
            GameEvent marketBoom = new GameEvent("Market Boom", () =>
            {
                node.Stats.EconomicProsperity += 20f;
                node.Stats.ResourceAvailability += 10f;
                Console.WriteLine($"Market Boom in {node.Name}! Economic Prosperity and Resources increased.");
            });

            EventManager.TriggerEvent(marketBoom);
        }
    }
}

// Explanation:
// EconomySystem Class: Manages the economic aspects of each node, updating prosperity based on resource availability and population density. It also introduces random economic events that can positively or negatively impact the economy.