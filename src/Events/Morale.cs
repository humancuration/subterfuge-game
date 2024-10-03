private void ApplyEffect(string statName, float value)
{
    switch (statName.ToLower())
    {
        case "morale":
            foreach (var waypoint in gameManager.Waypoints)
            {
                waypoint.Stats.Morale = Math.Clamp(waypoint.Stats.Morale + value, 0f, 100f);
                // Potentially trigger a new event if morale crosses a threshold
                if (waypoint.Stats.Morale > 80f)
                {
                    // Example: Trigger a celebration event
                    var celebrationEvent = allEvents.Find(e => e.Id == "event_celebration");
                    if (celebrationEvent != null && celebrationEvent.Conditions.IsMet(gameManager))
                    {
                        TriggerEvent(celebrationEvent);
                    }
                }
            }
            break;
        // Handle other stats similarly
        default:
            GD.PrintErr($"Unknown effect stat: {statName}");
            break;
    }
}

// o1 mini
// 7. Cascading Effects and Branching
// The ComplexEventSystem already handles triggering subsequent events based on player choices. To ensure cascading effects across multiple nodes and systems, integrate event outcomes with various game elements.
// Example: Implementing a Cascading Effect
// Suppose an event choice increases morale in multiple nodes, which in turn affects PoliticalStability and triggers new events.
// Explanation
// Threshold Checks: After applying an effect, check if certain stats exceed thresholds to trigger new events.
// Cascading Events: Ensure that events can lead to other events, creating a web of interconnected occurrences.
