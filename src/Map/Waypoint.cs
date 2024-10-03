using Godot;
using System;
using System.Collections.Generic;
using Models;

public class Waypoint : Node2D, IUpdatable
{
    // Existing properties
    public int Id { get; private set; }
    public string WaypointName { get; private set; }
    public TerrainFeature TerrainFeature { get; private set; }
    public PopulationCharacteristics Population { get; private set; }
    public WaypointStats Stats { get; private set; }

    // New properties
    public List<Waypoint> ConnectedWaypoints { get; private set; } = new List<Waypoint>();
    public List<string> Events { get; private set; } = new List<string>();
    public Dictionary<string, Resource> Resources { get; private set; } = new Dictionary<string, Resource>();

    public Waypoint(int id, string name, Vector2 position, TerrainFeature terrainFeature)
    {
        Id = id;
        WaypointName = name;
        Position = position;
        TerrainFeature = terrainFeature;
        Stats = new WaypointStats();
        Population = new PopulationCharacteristics();
    }

    // Existing Update method
    public void Update(float deltaTime)
    {
        // Update stats based on modifiers and environment
        Stats.Update(deltaTime, TerrainFeature, Population);

        // Update population based on stats and environment
        Population.Update(deltaTime, Stats, TerrainFeature);

        // Additional update logic for the node
        UpdateNodeLogic(deltaTime);

        // Trigger events based on certain conditions
        CheckAndTriggerEvents();

        // Update resource production
        UpdateResourceProduction(deltaTime);
    }

    // New method to connect waypoints
    public void ConnectTo(Waypoint waypoint)
    {
        if (!ConnectedWaypoints.Contains(waypoint))
        {
            ConnectedWaypoints.Add(waypoint);
            waypoint.ConnectTo(this); // Ensure bidirectional connection
        }
    }

    // New method to disconnect waypoints
    public void DisconnectFrom(Waypoint waypoint)
    {
        if (ConnectedWaypoints.Contains(waypoint))
        {
            ConnectedWaypoints.Remove(waypoint);
            waypoint.DisconnectFrom(this);
        }
    }

    // New method to add an event
    public void AddEvent(string eventDescription)
    {
        if (!Events.Contains(eventDescription))
        {
            Events.Add(eventDescription);
        }
    }

    // New method to remove an event
    public bool RemoveEvent(string eventDescription)
    {
        return Events.Remove(eventDescription);
    }

    // New method to handle node-specific logic
    private void UpdateNodeLogic(float deltaTime)
    {
        // Example: Regenerate resources over time
        Stats.ResourceAvailability = Math.Clamp(Stats.ResourceAvailability + deltaTime * 0.1f, 0f, 100f);
    }

    // New method to check and trigger events
    private void CheckAndTriggerEvents()
    {
        if (Stats.Morale < 20f && !Events.Contains("LowMorale"))
        {
            AddEvent("LowMorale");
            EventManager.TriggerEvent("LowMorale", this); // Trigger the event
        }

        if (Population.Size > 1000 && !Events.Contains("PopulationBoom"))
        {
            AddEvent("PopulationBoom");
            EventManager.TriggerEvent("PopulationBoom", this); // Trigger the event
        }

        // Add more event checks as needed
        if (Stats.SocialHappiness < 30f && !Events.Contains("SocialUnrest"))
        {
            AddEvent("SocialUnrest");
            EventManager.TriggerEvent("SocialUnrest", this);
        }
    }

    // Method to retrieve a connected waypoint by ID
    public Waypoint GetConnectedWaypointById(int id)
    {
        return ConnectedWaypoints.Find(wp => wp.Id == id);
    }

    // Method to calculate distance to another waypoint
    public float DistanceTo(Waypoint other)
    {
        return Position.DistanceTo(other.Position);
    }

    // Method to serialize waypoint data
    public Dictionary<string, object> SerializeWaypoint()
    {
        var data = new Dictionary<string, object>
        {
            { "Id", Id },
            { "WaypointName", WaypointName },
            { "Position", new float[] { Position.x, Position.y } },
            { "TerrainFeature", new Dictionary<string, object>
                {
                    { "Name", TerrainFeature.Name },
                    { "MovementCost", TerrainFeature.MovementCost },
                    { "ResourceModifier", TerrainFeature.ResourceModifier }
                }
            },
            { "Population", new Dictionary<string, object>
                {
                    { "Size", Population.Size },
                    { "GrowthRate", Population.GrowthRate },
                    { "EducationLevel", Population.EducationLevel },
                    { "HealthLevel", Population.HealthLevel }
                }
            },
            { "Stats", Stats.SerializeStats() },
            { "ConnectedWaypoints", ConnectedWaypoints.ConvertAll(wp => wp.Id) },
            { "Events", Events },
            { "Resources", Resources.ToDictionary(r => r.Key, r => r.Value.Amount) }
        };

        return data;
    }

    // Method to deserialize waypoint data
    public void DeserializeWaypoint(Dictionary<string, object> data, Dictionary<int, Waypoint> waypointLookup)
    {
        if (data.TryGetValue("Id", out object idObj))
        {
            Id = Convert.ToInt32(idObj);
        }

        if (data.TryGetValue("WaypointName", out object nameObj))
        {
            WaypointName = nameObj.ToString();
        }

        if (data.TryGetValue("Position", out object posObj) && posObj is float[] posArray && posArray.Length == 2)
        {
            Position = new Vector2(posArray[0], posArray[1]);
        }

        if (data.TryGetValue("TerrainFeature", out object terrainObj) && terrainObj is Dictionary<string, object> terrainDict)
        {
            TerrainFeature = new TerrainFeature
            {
                Name = terrainDict["Name"].ToString(),
                MovementCost = Convert.ToSingle(terrainDict["MovementCost"]),
                ResourceModifier = Convert.ToSingle(terrainDict["ResourceModifier"])
            };
        }

        if (data.TryGetValue("Population", out object popObj) && popObj is Dictionary<string, object> popDict)
        {
            Population = new PopulationCharacteristics
            {
                Size = Convert.ToInt32(popDict["Size"]),
                GrowthRate = Convert.ToSingle(popDict["GrowthRate"]),
                EducationLevel = Convert.ToSingle(popDict["EducationLevel"]),
                HealthLevel = Convert.ToSingle(popDict["HealthLevel"])
            };
        }

        if (data.TryGetValue("Stats", out object statsObj) && statsObj is Dictionary<string, float> statsDict)
        {
            Stats.DeserializeStats(statsDict);
        }

        if (data.TryGetValue("ConnectedWaypoints", out object connectedObj) && connectedObj is List<object> connectedList)
        {
            foreach (var wpIdObj in connectedList)
            {
                int wpId = Convert.ToInt32(wpIdObj);
                if (waypointLookup.TryGetValue(wpId, out Waypoint connectedWaypoint))
                {
                    ConnectTo(connectedWaypoint);
                }
            }
        }

        if (data.TryGetValue("Events", out object eventsObj) && eventsObj is List<object> eventsList)
        {
            Events.Clear();
            foreach (var evt in eventsList)
            {
                Events.Add(evt.ToString());
            }
        }

        if (data.TryGetValue("Resources", out object resourcesObj) && resourcesObj is Dictionary<string, object> resourcesDict)
        {
            foreach (var kvp in resourcesDict)
            {
                string resourceName = kvp.Key;
                int resourceAmount = Convert.ToInt32(kvp.Value);
                Resources[resourceName] = new Resource(resourceName, resourceAmount);
            }
        }
    }

    private void UpdateResourceProduction(float deltaTime)
    {
        // Implement resource production logic based on terrain, population, etc.
        // Example:
        if (TerrainFeature.Name == "Forest")
        {
            if (!Resources.ContainsKey("Wood"))
            {
                Resources["Wood"] = new Resource("Wood", 0);
            }

            // Increase wood production based on technological advancement
            float productionMultiplier = 1 + Stats.TechnologicalAdvancement / 100f; // Example: 1% increase per tech level
            Resources["Wood"].Amount += (int)(deltaTime * Population.Size * 0.1f * productionMultiplier);
        }
    }
}
