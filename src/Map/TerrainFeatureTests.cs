
using NUnit.Framework;
using Models;
using System.Collections.Generic;

namespace Tests
{
    [TestFixture]
    public class TerrainFeatureTests
    {
        [Test]
        public void SerializeDeserializeTerrainFeature_ShouldMaintainData()
        {
            var terrain = new TerrainFeature("Desert", 3.0f, 0.5f);
            var serialized = terrain.SerializeTerrainFeature();
            var deserialized = TerrainFeature.DeserializeTerrainFeature(serialized);

            Assert.AreEqual(terrain.Name, deserialized.Name);
            Assert.AreEqual(terrain.MovementCost, deserialized.MovementCost);
            Assert.AreEqual(terrain.ResourceModifier, deserialized.ResourceModifier);
        }

        [Test]
        public void TerrainFeatureConstructor_ShouldSetProperties()
        {
            var terrain = new TerrainFeature("Swamp", 2.5f, 0.8f);
            Assert.AreEqual("Swamp", terrain.Name);
            Assert.AreEqual(2.5f, terrain.MovementCost);
            Assert.AreEqual(0.8f, terrain.ResourceModifier);
        }
    }
}