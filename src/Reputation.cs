// File: Models/Reputation.cs
// Reputation System
// Implement a reputation system that tracks the player's standing with different factions and NPCs, affecting dialogues, quests, and interactions.

using System.Collections.Generic;

namespace Models
{
    public class Reputation
    {
        public Dictionary<string, int> FactionReputation { get; set; }
        public Dictionary<string, int> NPCReputation { get; set; }

        public Reputation()
        {
            FactionReputation = new Dictionary<string, int>();
            NPCReputation = new Dictionary<string, int>();
        }

        public void SetFactionReputation(string factionName, int value)
        {
            FactionReputation[factionName] = value;
        }

        public int GetFactionReputation(string factionName)
        {
            return FactionReputation.ContainsKey(factionName) ? FactionReputation[factionName] : 0;
        }

        public void SetNPCReputation(string npcName, int value)
        {
            NPCReputation[npcName] = value;
        }

        public int GetNPCReputation(string npcName)
        {
            return NPCReputation.ContainsKey(npcName) ? NPCReputation[npcName] : 0;
        }
    }
}

// File: Systems/ReputationSystem.cs
using Models;

namespace Systems
{
    public class ReputationSystem : IUpdatable
    {
        private Reputation playerReputation;

        public ReputationSystem()
        {
            playerReputation = new Reputation();
        }

        public void Update(float deltaTime)
        {
            // Update reputation based on player actions and events
        }

        public void UpdateFactionReputation(string factionName, int value)
        {
            int currentRep = playerReputation.GetFactionReputation(factionName);
            playerReputation.SetFactionReputation(factionName, currentRep + value);
        }

        public void UpdateNPCReputation(string npcName, int value)
        {
            int currentRep = playerReputation.GetNPCReputation(npcName);
            playerReputation.SetNPCReputation(npcName, currentRep + value);
        }
    }
}