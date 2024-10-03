
using System;
using System.Collections.Generic;

namespace Models
{
    public class WaypointStats
    {
        // Existing Core Stats
        public float Morale { get; set; }
        public float ResourceAvailability { get; set; }
        public float PoliticalStability { get; set; }
        public float EnvironmentalHealth { get; set; }
        public float EconomicProsperity { get; set; }
        public float PopulationDensity { get; set; }
        public float Radicalization { get; set; }
        public float HealthRisk { get; set; }

        // Existing Additional Stats
        public float CrimeRate { get; set; }
        public float TechnologicalLevel { get; set; }
        public float CulturalDevelopment { get; set; }
        public float InfluenceSpread { get; set; }
        public float MedicalResources { get; set; }

        // Existing New Stats
        public float Education { get; set; }
        public float Infrastructure { get; set; }
        public float Tourism { get; set; }

        // Dictionary to store custom stats for easy expansion
        private Dictionary<string, float> customStats = new Dictionary<string, float>();

        // Stat change events
        public event Action<string, float, float> OnStatChanged;

        // Constructor to initialize stats with default values
        public WaypointStats()
        {
            InitializeStats();
        }

        private void InitializeStats()
        {
            // Set default values for all stats
            Morale = 50f;
            ResourceAvailability = 50f;
            PoliticalStability = 50f;
            EnvironmentalHealth = 50f;
            EconomicProsperity = 50f;
            PopulationDensity = 50f;
            Radicalization = 0f;
            HealthRisk = 0f;
            CrimeRate = 10f;
            TechnologicalLevel = 50f;
            CulturalDevelopment = 50f;
            InfluenceSpread = 0f;
            MedicalResources = 50f;
            Education = 50f;
            Infrastructure = 50f;
            Tourism = 0f;
        }

        // Update method to adjust stats over time
        public void Update(float deltaTime, TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            UpdateStat(nameof(Morale), CalculateMoraleChange(terrainFeature, population), deltaTime);
            UpdateStat(nameof(HealthRisk), CalculateHealthRiskChange(terrainFeature, population), deltaTime);
            UpdateStat(nameof(EconomicProsperity), CalculateEconomicProsperityChange(terrainFeature, population), deltaTime);
            UpdateStat(nameof(Education), CalculateEducationChange(terrainFeature, population), deltaTime);
            // Similarly update other stats...
        }

        private void UpdateStat(string statName, float change, float deltaTime)
        {
            float oldValue = GetStatValue(statName);
            float newValue = Math.Clamp(oldValue + change * deltaTime, 0f, 100f);
            SetStatValue(statName, newValue);

            if (Math.Abs(newValue - oldValue) > 0.01f)
            {
                OnStatChanged?.Invoke(statName, oldValue, newValue);
            }
        }

        private float CalculateMoraleChange(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Enhanced calculation considering population and terrain
            float baseChange = (EconomicProsperity + PoliticalStability + EnvironmentalHealth) / 3;
            float populationFactor = population.GrowthRate * 0.5f;
            float terrainFactor = terrainFeature.ResourceModifier * 0.3f;
            return baseChange + populationFactor + terrainFactor - Morale;
        }

        private float CalculateHealthRiskChange(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // Enhanced calculation considering population health and terrain
            float baseChange = (100 - EnvironmentalHealth - MedicalResources) / 2;
            float populationHealthFactor = population.HealthLevel * 0.4f;
            float terrainHealthFactor = terrainFeature.Name == "Polluted" ? 20f : 0f;
            return baseChange - populationHealthFactor - terrainHealthFactor - HealthRisk;
        }

        private float CalculateEconomicProsperityChange(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // New calculation for Economic Prosperity
            float baseChange = (ResourceAvailability + Infrastructure) / 2;
            float technologyFactor = TechnologicalLevel * 0.3f;
            return baseChange + technologyFactor - EconomicProsperity;
        }

        private float CalculateEducationChange(TerrainFeature terrainFeature, PopulationCharacteristics population)
        {
            // New calculation for Education
            float baseChange = EducationLevelImpact(population.EducationLevel);
            float culturalFactor = CulturalDevelopment * 0.2f;
            return baseChange + culturalFactor - Education;
        }

        private float EducationLevelImpact(float educationLevel)
        {
            // Example impact of education level on education stat
            return educationLevel * 0.5f;
        }

        // Methods to get and set stat values, including custom stats
        public float GetStatValue(string statName)
        {
            if (customStats.TryGetValue(statName, out float value))
            {
                return value;
            }

            return GetType().GetProperty(statName)?.GetValue(this) as float? ?? 0f;
        }

        public void SetStatValue(string statName, float value)
        {
            if (GetType().GetProperty(statName) != null)
            {
                GetType().GetProperty(statName).SetValue(this, value);
            }
            else
            {
                customStats[statName] = value;
            }
        }

        // Method to add a new custom stat
        public void AddCustomStat(string statName, float initialValue = 0f)
        {
            if (!customStats.ContainsKey(statName))
            {
                customStats[statName] = initialValue;
            }
        }

        // Method to remove a custom stat
        public bool RemoveCustomStat(string statName)
        {
            return customStats.Remove(statName);
        }

        // Method to reset all stats to default values
        public void ResetStats()
        {
            InitializeStats();
            customStats.Clear();
        }

        // Method to serialize stats to a dictionary (for saving/loading)
        public Dictionary<string, float> SerializeStats()
        {
            var dict = new Dictionary<string, float>();

            foreach (var prop in GetType().GetProperties())
            {
                if (prop.PropertyType == typeof(float))
                {
                    dict[prop.Name] = (float)prop.GetValue(this);
                }
            }

            foreach (var kvp in customStats)
            {
                dict[kvp.Key] = kvp.Value;
            }

            return dict;
        }

        // Method to deserialize stats from a dictionary (for loading)
        public void DeserializeStats(Dictionary<string, float> stats)
        {
            foreach (var kvp in stats)
            {
                SetStatValue(kvp.Key, kvp.Value);
            }
        }
    }
}
// This enhanced version includes the following improvements:
// Added new stats: Education, Infrastructure, and Tourism.
// Implemented a dictionary for custom stats, allowing easy addition of new stats without modifying the class.
// Added an event system to notify when stats change significantly.
// Implemented a constructor to initialize stats with default values.
// Created methods to get and set stat values, which work for both predefined and custom stats.
// Added a method to easily add new custom stats.
// Implemented example calculations for Morale and HealthRisk changes.
// Used Math.Clamp to ensure stat values stay within the 0-100 range.
// This version provides a more flexible and extensible NodeStats class that can easily accommodate future additions to the game's mechanics. It also provides better encapsulation and event-driven architecture, which can be useful for updating UI elements or triggering game events based on stat changes.