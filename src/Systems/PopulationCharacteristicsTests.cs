
using NUnit.Framework;
using Models;
using System.Collections.Generic;

namespace Tests
{
    [TestFixture]
    public class PopulationCharacteristicsTests
    {
        private PopulationCharacteristics population;
        private WaypointStats stats;
        private TerrainFeature terrain;

        [SetUp]
        public void Setup()
        {
            population = new PopulationCharacteristics();
            stats = new WaypointStats();
            terrain = new TerrainFeature("Plains", 1.0f, 1.0f);
        }

        [Test]
        public void InitializePopulation_ShouldHaveDefaultValues()
        {
            Assert.AreEqual(1000, population.Size);
            Assert.AreEqual(1.0f, population.GrowthRate);
            Assert.AreEqual(50f, population.EducationLevel);
            Assert.AreEqual(50f, population.HealthLevel);
        }

        [Test]
        public void Update_ShouldIncreasePopulationSize()
        {
            population.Update(10f, stats, terrain); // 10 seconds
            int expectedSize = (int)Mathf.Clamp(1000 + 1000 * (1.0f / 100f) * 10f, 0, 1000000);
            Assert.AreEqual(expectedSize, population.Size);
        }

        [Test]
        public void AdjustGrowthRate_ShouldModifyGrowthBasedOnStats()
        {
            stats.SetStatValue("ResourceAvailability", 80f);
            stats.SetStatValue("HealthRisk", 20f);
            population.Update(1.0f, stats, terrain);
            Assert.AreEqual(1.0f + (80f / 100f) - (20f / 100f), population.GrowthRate);
        }

        [Test]
        public void AdjustHealth_ShouldModifyHealthLevel()
        {
            stats.SetStatValue("HealthRisk", 30f);
            stats.SetStatValue("MedicalResources", 70f);
            population.Update(1.0f, stats, terrain);
            float expectedHealth = Mathf.Clamp(50f + (100f - 30f) * 0.01f, 0f, 100f);
            Assert.AreEqual(expectedHealth, population.HealthLevel);
        }

        [Test]
        public void AdjustEducation_ShouldModifyEducationLevel()
        {
            stats.SetStatValue("Education", 60f);
            stats.SetStatValue("CulturalDevelopment", 40f);
            population.Update(1.0f, stats, terrain);
            float expectedEducation = Mathf.Clamp(50f + (60f * 0.02f) + (40f * 0.01f), 0f, 100f);
            Assert.AreEqual(expectedEducation, population.EducationLevel);
        }

        [Test]
        public void SerializeDeserializePopulation_ShouldMaintainValues()
        {
            population.Size = 2000;
            population.GrowthRate = 2.0f;
            population.EducationLevel = 70f;
            population.HealthLevel = 80f;

            var serialized = population.SerializePopulation();
            var newPopulation = new PopulationCharacteristics();
            newPopulation.DeserializePopulation(serialized);

            Assert.AreEqual(2000, newPopulation.Size);
            Assert.AreEqual(2.0f, newPopulation.GrowthRate);
            Assert.AreEqual(70f, newPopulation.EducationLevel);
            Assert.AreEqual(80f, newPopulation.HealthLevel);
        }
    }
}