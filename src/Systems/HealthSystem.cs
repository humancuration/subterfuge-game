// 3. Health System
// The Health System tracks the health status of populations within nodes, handling factors like disease spread and healthcare availability.
// 3.1. Health System Class
// File: Systems/HealthSystem.cs

using System;
using System.Collections.Generic;
using Models;

namespace Systems
{
    public class HealthSystem : IUpdatable
    {
        private List<Node> nodes;
        private Random rand;

        public HealthSystem(List<Node> nodeList)
        {
            nodes = nodeList;
            rand = new Random();
        }

        public void Update(float deltaTime)
        {
            foreach (var node in nodes)
            {
                UpdateHealth(node, deltaTime);
            }
        }

        private void UpdateHealth(Node node, float deltaTime)
        {
            // Example: Decrease Health based on Pollution and Increase based on Medical Resources
            float healthChange = deltaTime * ( -node.Stats.EnvironmentalHealth * 0.05f + node.Stats.MedicalResources * 0.1f );
            node.Stats.HealthRisk += healthChange;

            // Ensure stats remain within logical bounds
            node.Stats.HealthRisk = Math.Clamp(node.Stats.HealthRisk, 0f, 100f);

            // Random health events
            if (rand.NextDouble() < 0.003) // 0.3% chance per update
            {
                TriggerHealthEvent(node);
            }
        }

        private void TriggerHealthEvent(Node node)
        {
            // Example health event: Outbreak
            GameEvent outbreak = new GameEvent("Disease Outbreak", () =>
            {
                node.Stats.HealthRisk += 30f;
                node.Stats.PopulationDensity -= 5f; // Loss due to disease
                Console.WriteLine($"Disease Outbreak in {node.Name}! Health Risk increased and population decreased.");
            });

            EventManager.TriggerEvent(outbreak);
        }
    }
}

// Explanation:
// HealthSystem Class: Monitors and updates the health-related stats of each node. It factors in environmental health and medical resources to determine health risks. Additionally, it can trigger random health events like disease outbreaks.