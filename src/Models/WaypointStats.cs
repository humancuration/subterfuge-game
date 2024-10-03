using System;
using System.Collections.Generic;

namespace Models
{
    public class WaypointStats
    {
        private Dictionary<string, float> stats;

        public float Morale
        {
            get { return stats["Morale"]; }
            set { stats["Morale"] = value; }
        }

        public float ResourceAvailability
        {
            get { return stats["ResourceAvailability"]; }
            set { stats["ResourceAvailability"] = value; }
        }

        // New stats
        public float PopulationGrowth
        {
            get { return stats["PopulationGrowth"]; }
            set { stats["PopulationGrowth"] = value; }
        }

        public float TechnologicalAdvancement
        {
            get { return stats["TechnologicalAdvancement"]; }
            set { stats["TechnologicalAdvancement"] = value; }
        }

        public float SocialHappiness
        {
            get { return stats["SocialHappiness"]; }
            set { stats["SocialHappiness"] = value; }
        }

        public WaypointStats()
        {
            InitializeStats();
        }

        private void InitializeStats()
        {
            stats = new Dictionary<string, float>
            {
                { "Morale", 50f },
                { "ResourceAvailability", 50f },
                { "PopulationGrowth", 0f }, // Initialize new stats
                { "TechnologicalAdvancement", 0f },
                { "SocialHappiness", 50f }
            };
        }

        public void Update(float deltaTime, TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Update existing stats
            Morale += CalculateMoraleChange(terrainFeature, population) * deltaTime;
            ResourceAvailability += CalculateResourceAvailabilityChange(terrainFeature, population) * deltaTime;

            // Update new stats
            PopulationGrowth = CalculatePopulationGrowth(terrainFeature, population);
            TechnologicalAdvancement += CalculateTechnologicalAdvancement(terrainFeature, population) * deltaTime;
            SocialHappiness += CalculateSocialHappinessChange(terrainFeature, population) * deltaTime;
        }

        // Existing methods for calculating stat changes
        private float CalculateMoraleChange(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Implement logic to calculate morale change based on terrain and population
            return 0f; // Placeholder
        }

        private float CalculateResourceAvailabilityChange(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Implement logic to calculate resource availability change based on terrain and population
            return 0f; // Placeholder
        }

        // New methods for calculating stat changes
        private float CalculatePopulationGrowth(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Implement logic to calculate population growth based on terrain and population
            return 0f; // Placeholder
        }

        private float CalculateTechnologicalAdvancement(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Implement logic to calculate technological advancement based on terrain and population
            return 0f; // Placeholder
        }

        private float CalculateSocialHappinessChange(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Implement logic to calculate social happiness change based on terrain and population
            return 0f; // Placeholder
        }

        public float GetStatValue(string statName)
        {
            if (stats.ContainsKey(statName))
            {
                return stats[statName];
            }
            else
            {
                return 0f; // Or throw an exception if the stat doesn't exist
            }
        }

        public void SetStatValue(string statName, float value)
        {
            if (stats.ContainsKey(statName))
            {
                stats[statName] = value;
            }
            else
            {
                // Handle the case where the stat doesn't exist
            }
        }

        public void AddCustomStat(string statName, float initialValue = 0f)
        {
            if (!stats.ContainsKey(statName))
            {
                stats.Add(statName, initialValue);
            }
        }

        public bool RemoveCustomStat(string statName)
        {
            return stats.Remove(statName);
        }

        public void ResetStats()
        {
            InitializeStats();
        }

        public Dictionary<string, float> SerializeStats()
        {
            return new Dictionary<string, float>(stats); // Return a copy of the stats dictionary
        }

        public void DeserializeStats(Dictionary<string, float> stats)
        {
            this.stats = new Dictionary<string, float>(stats); // Create a new dictionary from the deserialized data
        }
    }
}