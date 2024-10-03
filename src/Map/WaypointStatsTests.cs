
using NUnit.Framework;
using Models;
using System;

namespace Tests
{
    [TestFixture]
    public class WaypointStatsTests
    {
        private WaypointStats stats;
        private TerrainFeature terrain;
        private PopulationCharacteristics population;

        [SetUp]
        public void Setup()
        {
            stats = new WaypointStats();
            terrain = new TerrainFeature("Forest", 1.2f, 1.0f);
            population = new PopulationCharacteristics();
        }

        [Test]
        public void InitializeStats_ShouldHaveDefaultValues()
        {
            Assert.AreEqual(50f, stats.Morale);
            Assert.AreEqual(50f, stats.ResourceAvailability);
            Assert.AreEqual(50f, stats.PoliticalStability);
            Assert.AreEqual(50f, stats.EnvironmentalHealth);
            Assert.AreEqual(50f, stats.EconomicProsperity);
            Assert.AreEqual(50f, stats.PopulationDensity);
            Assert.AreEqual(0f, stats.Radicalization);
            Assert.AreEqual(0f, stats.HealthRisk);
            Assert.AreEqual(10f, stats.CrimeRate);
            Assert.AreEqual(50f, stats.TechnologicalLevel);
            Assert.AreEqual(50f, stats.CulturalDevelopment);
            Assert.AreEqual(0f, stats.InfluenceSpread);
            Assert.AreEqual(50f, stats.MedicalResources);
            Assert.AreEqual(50f, stats.Education);
            Assert.AreEqual(50f, stats.Infrastructure);
            Assert.AreEqual(0f, stats.Tourism);
        }

        [Test]
        public void Update_ShouldAdjustMoraleCorrectly()
        {
            stats.Update(1.0f, terrain, population);
            float expectedChange = (50f + 50f + 50f) / 3 + (population.GrowthRate * 0.5f) + (terrain.ResourceModifier * 0.3f) - stats.Morale;
            float expectedMorale = Math.Clamp(stats.Morale + expectedChange * 1.0f, 0f, 100f);
            Assert.AreEqual(expectedMorale, stats.Morale);
        }

        [Test]
        public void GetSetStatValue_ShouldHandlePredefinedStats()
        {
            stats.SetStatValue("Morale", 80f);
            Assert.AreEqual(80f, stats.GetStatValue("Morale"));
        }

        [Test]
        public void GetSetStatValue_ShouldHandleCustomStats()
        {
            stats.SetStatValue("CustomStat", 25f);
            Assert.AreEqual(25f, stats.GetStatValue("CustomStat"));
        }

        [Test]
        public void AddCustomStat_ShouldAddNewStat()
        {
            stats.AddCustomStat("NewStat", 30f);
            Assert.AreEqual(30f, stats.GetStatValue("NewStat"));
        }

        [Test]
        public void RemoveCustomStat_ShouldRemoveExistingStat()
        {
            stats.AddCustomStat("TempStat", 40f);
            bool removed = stats.RemoveCustomStat("TempStat");
            Assert.IsTrue(removed);
            Assert.AreEqual(0f, stats.GetStatValue("TempStat"));
        }

        [Test]
        public void OnStatChanged_ShouldTriggerEvent()
        {
            string changedStat = null;
            float oldValue = 0f;
            float newValue = 0f;

            stats.OnStatChanged += (stat, oldVal, newVal) =>
            {
                changedStat = stat;
                oldValue = oldVal;
                newValue = newVal;
            };

            stats.SetStatValue("Morale", 60f);
            stats.Update(1.0f, terrain, population);

            Assert.AreEqual("Morale", changedStat);
            Assert.AreNotEqual(50f, oldValue);
            Assert.AreNotEqual(50f, newValue);
        }

        [Test]
        public void SerializeDeserializeStats_ShouldMaintainStatValues()
        {
            stats.SetStatValue("Morale", 70f);
            stats.AddCustomStat("CustomStat", 35f);

            var serialized = stats.SerializeStats();
            var newStats = new WaypointStats();
            newStats.DeserializeStats(serialized);

            Assert.AreEqual(70f, newStats.GetStatValue("Morale"));
            Assert.AreEqual(35f, newStats.GetStatValue("CustomStat"));
        }
    }
}