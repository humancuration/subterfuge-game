// File: Models/Faction.cs
// Faction System
// Introduce factions that NPCs can belong to, influencing their behaviors, dialogues, and interactions with each other and the player.

using System.Collections.Generic;

namespace Models
{
    public class Faction
    {
        public string Name { get; set; }
        public Dictionary<string, int> Relations { get; set; }

        public Faction(string name)
        {
            Name = name;
            Relations = new Dictionary<string, int>();
        }

        public void SetRelation(string factionName, int value)
        {
            Relations[factionName] = value;
        }

        public int GetRelation(string factionName)
        {
            return Relations.ContainsKey(factionName) ? Relations[factionName] : 0;
        }
    }
}

// File: Systems/FactionSystem.cs
using System.Collections.Generic;
using Models;

namespace Systems
{
    public class FactionSystem : IUpdatable
    {
        private Dictionary<string, Faction> factions;

        public FactionSystem()
        {
            factions = new Dictionary<string, Faction>();
        }

        public void Update(float deltaTime)
        {
            // Update faction relations based on events and actions
        }

        public void AddFaction(Faction faction)
        {
            factions[faction.Name] = faction;
        }

        public Faction GetFaction(string factionName)
        {
            return factions.ContainsKey(factionName) ? factions[factionName] : null;
        }
    }
}