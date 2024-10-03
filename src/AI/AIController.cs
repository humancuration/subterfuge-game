// File: src/AI/AIController.cs
using Godot;
using System.Collections.Generic;

public class AIController : Node
{
    private Waypoint currentWaypoint;
    private List<Waypoint> visitedWaypoints = new List<Waypoint>();

    public void SetStartWaypoint(Waypoint startWaypoint)
    {
        currentWaypoint = startWaypoint;
        visitedWaypoints.Add(currentWaypoint);
    }

    public void MoveToNextWaypoint()
    {
        if (currentWaypoint.ConnectedWaypoints.Count == 0)
            return;

        // Simple random movement
        Random rnd = new Random();
        Waypoint nextWaypoint = currentWaypoint.ConnectedWaypoints[rnd.Next(currentWaypoint.ConnectedWaypoints.Count)];
        currentWaypoint = nextWaypoint;

        if (!visitedWaypoints.Contains(currentWaypoint))
            visitedWaypoints.Add(currentWaypoint);

        GD.Print($"AI moved to {currentWaypoint.WaypointName}");
    }
}