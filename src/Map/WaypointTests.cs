
using NUnit.Framework;
using Models;
using System.Collections.Generic;
using Godot;

namespace Tests
{
    [TestFixture]
    public class WaypointTests
    {
        private Waypoint waypoint1;
        private Waypoint waypoint2;
        private TerrainFeature terrain;
        private PopulationCharacteristics population;

        [SetUp]
        public void Setup()
        {
            terrain = new TerrainFeature("Mountain", 2.0f, 1.5f);
            population = new PopulationCharacteristics();
            waypoint1 = new Waypoint(1, "Alpha", new Vector2(0, 0), terrain);
            waypoint2 = new Waypoint(2, "Beta", new Vector2(100, 100), terrain);
        }

        [Test]
        public void ConnectTo_ShouldEstablishBidirectionalConnection()
        {
            waypoint1.ConnectTo(waypoint2);
            Assert.Contains(waypoint2, waypoint1.ConnectedWaypoints);
            Assert.Contains(waypoint1, waypoint2.ConnectedWaypoints);
        }

        [Test]
        public void DisconnectFrom_ShouldRemoveBidirectionalConnection()
        {
            waypoint1.ConnectTo(waypoint2);
            waypoint1.DisconnectFrom(waypoint2);
            Assert.IsFalse(waypoint1.ConnectedWaypoints.Contains(waypoint2));
            Assert.IsFalse(waypoint2.ConnectedWaypoints.Contains(waypoint1));
        }

        [Test]
        public void AddEvent_ShouldAddNewEvent()
        {
            waypoint1.AddEvent("NewEvent");
            Assert.Contains("NewEvent", waypoint1.Events);
        }

        [Test]
        public void RemoveEvent_ShouldRemoveExistingEvent()
        {
            waypoint1.AddEvent("EventToRemove");
            bool removed = waypoint1.RemoveEvent("EventToRemove");
            Assert.IsTrue(removed);
            Assert.IsFalse(waypoint1.Events.Contains("EventToRemove"));
        }

        [Test]
        public void Update_ShouldModifyStatsAndPopulation()
        {
            waypoint1.Update(1.0f);
            Assert.Greater(waypoint1.Stats.ResourceAvailability, 50f);
            Assert.Greater(waypoint1.Population.Size, 1000);
        }

        [Test]
        public void SerializeDeserializeWaypoint_ShouldMaintainDataIntegrity()
        {
            waypoint1.ConnectTo(waypoint2);
            waypoint1.AddEvent("TestEvent");
            waypoint1.Stats.SetStatValue("Morale", 80f);
            waypoint1.Population.Size = 1500;

            var serialized = waypoint1.SerializeWaypoint();

            // Create a lookup for connected waypoints
            var waypointLookup = new Dictionary<int, Waypoint>
            {
                { waypoint2.Id, waypoint2 }
            };

            var newWaypoint = new Waypoint(0, "", new Vector2(), null); // Temporary values
            newWaypoint.DeserializeWaypoint(serialized, waypointLookup);

            Assert.AreEqual(1, newWaypoint.Id);
            Assert.AreEqual("Alpha", newWaypoint.WaypointName);
            Assert.AreEqual(new Vector2(0, 0), newWaypoint.Position);
            Assert.AreEqual("Mountain", newWaypoint.TerrainFeature.Name);
            Assert.AreEqual(2.0f, newWaypoint.TerrainFeature.MovementCost);
            Assert.AreEqual(1.5f, newWaypoint.TerrainFeature.ResourceModifier);
            Assert.AreEqual(1500, newWaypoint.Population.Size);
            Assert.AreEqual(80f, newWaypoint.Stats.GetStatValue("Morale"));
            Assert.Contains(waypoint2, newWaypoint.ConnectedWaypoints);
            Assert.Contains("TestEvent", newWaypoint.Events);
        }

        [Test]
        public void DistanceTo_ShouldReturnCorrectDistance()
        {
            float distance = waypoint1.DistanceTo(waypoint2);
            Assert.AreEqual(Mathf.Sqrt(2 * 100 * 100), distance);
        }
    }
}