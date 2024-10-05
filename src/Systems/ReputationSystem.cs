using System;
using System.Collections.Generic;
using Models;

namespace Systems
{
    public class ReputationSystem : IUpdatable
    {
        private GameManager gameManager;

        public ReputationSystem(GameManager manager)
        {
            gameManager = manager;
        }

        public void Update(float deltaTime)
        {
            // Example: Gradually normalize reputation over time
            foreach (var node in gameManager.Nodes)
            {
                if (node.Stats.Reputation.ContainsKey("Player"))
                {
                    node.Stats.Reputation["Player"] = Math.Clamp(node.Stats.Reputation["Player"] + deltaTime * 0.1f, 0f, 100f);
                }
            }
        }

        public void ModifyReputation(NPC npc, float amount)
        {
            npc.ReputationWithPlayer = Math.Clamp(npc.ReputationWithPlayer + amount, 0f, 100f);
            // Optionally reflect this on the node's reputation
            if (npc.CurrentNode.Stats.Reputation.ContainsKey("Player"))
            {
                npc.CurrentNode.Stats.Reputation["Player"] = Math.Clamp(npc.CurrentNode.Stats.Reputation["Player"] + amount, 0f, 100f);
            }
        }

        public float GetReputation(NPC npc)
        {
            return npc.ReputationWithPlayer;
        }
    }
}