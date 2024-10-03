
using System;

namespace Models
{
    public class PopulationCharacteristics
    {
        public int Size { get; set; }
        public float GrowthRate { get; set; } // Percentage per update
        public float EducationLevel { get; set; } // 0 to 100
        public float HealthLevel { get; set; } // 0 to 100
        // Add more properties as needed

        public PopulationCharacteristics()
        {
            Size = 1000;
            GrowthRate = 1.0f; // 1% growth per update
            EducationLevel = 50f;
            HealthLevel = 50f;
        }

        public void Update(float deltaTime, WaypointStats stats, TerrainFeature terrainFeature)
        {
            // Implement population update logic based on stats and terrain
            AdjustGrowthRate(stats, terrainFeature);
            AdjustHealth(stats, terrainFeature);
            AdjustEducation(stats, terrainFeature);

            // Update population size based on growth rate
            Size = (int)Math.Clamp(Size + Size * (GrowthRate / 100f) * deltaTime, 0, 1000000);
        }

        private void AdjustGrowthRate(WaypointStats stats, TerrainFeature terrainFeature)
        {
            // Example: Growth rate influenced by HealthRisk and ResourceAvailability
            GrowthRate = 1.0f + (stats.ResourceAvailability / 100f) - (stats.HealthRisk / 100f);
            GrowthRate = Math.Clamp(GrowthRate, -5f, 5f); // Limit growth rate
        }

        private void AdjustHealth(WaypointStats stats, TerrainFeature terrainFeature)
        {
            // Example: Health level influenced by HealthRisk and MedicalResources
            HealthLevel += (100f - stats.HealthRisk) * 0.01f;
            HealthLevel = Math.Clamp(HealthLevel, 0f, 100f);
        }

        private void AdjustEducation(WaypointStats stats, TerrainFeature terrainFeature)
        {
            // Example: Education level influenced by Education stat and CulturalDevelopment
            EducationLevel += (stats.Education * 0.02f) + (stats.CulturalDevelopment * 0.01f);
            EducationLevel = Math.Clamp(EducationLevel, 0f, 100f);
        }

        // Method to serialize population data
        public Dictionary<string, object> SerializePopulation()
        {
            return new Dictionary<string, object>
            {
                { "Size", Size },
                { "GrowthRate", GrowthRate },
                { "EducationLevel", EducationLevel },
                { "HealthLevel", HealthLevel }
            };
        }

        // Method to deserialize population data
        public void DeserializePopulation(Dictionary<string, object> data)
        {
            if (data.TryGetValue("Size", out object sizeObj))
            {
                Size = Convert.ToInt32(sizeObj);
            }

            if (data.TryGetValue("GrowthRate", out object growthObj))
            {
                GrowthRate = Convert.ToSingle(growthObj);
            }

            if (data.TryGetValue("EducationLevel", out object eduObj))
            {
                EducationLevel = Convert.ToSingle(eduObj);
            }

            if (data.TryGetValue("HealthLevel", out object healthObj))
            {
                HealthLevel = Convert.ToSingle(healthObj);
            }
        }
    }
}

// Added methods for better population update logic and serialization/deserialization.
// The PopulationCharacteristics class now adjusts growth rate, health, and education levels based on stats and terrain.