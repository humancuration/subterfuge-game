using Godot;
using System;
using System.Collections.Generic;

/// <summary>
/// This class is responsible for generating the game map, including Waypoints and their connections.
/// </summary>
public class MapGenerator : Node
{
    private Random rnd = new Random();

    /// <summary>
    /// Generates a map with the specified number of nodes within the given map size.
    /// </summary>
    /// <param name="numberOfNodes">The desired number of Waypoints on the map.</param>
    /// <param name="mapSize">The overall size of the map (nodes will be generated within this area).</param>
    /// <returns>A list of generated Waypoints, representing the game map.</returns>
    public List<Waypoint> GenerateMap(int numberOfNodes, int mapSize)
    {
        // Initialize an empty list to store the generated Waypoints.
        List<Waypoint> mapNodes = new List<Waypoint>();

        // Generate terrain features (this part is not implemented yet, you'll need to add logic here).
        TerrainFeature[] terrainFeatures = GenerateTerrainFeatures(mapSize); 

        // Generate the specified number of Waypoints.
        for (int i = 0; i < numberOfNodes; i++)
        {
            // Generate a random position within the map bounds.
            Vector2 position = new Vector2(rnd.Next(-mapSize / 2, mapSize / 2), rnd.Next(-mapSize / 2, mapSize / 2));

            // Get the terrain feature at the generated position (not implemented yet).
            TerrainFeature terrainFeature = GetTerrainFeatureAtPosition(position, terrainFeatures);

            // Create a new Waypoint with a unique ID, name, position, and terrain.
            Waypoint newNode = new Waypoint(i, $"Node_{i}", position, terrainFeature);

            // Add the new Waypoint to the mapNodes list and as a child of this MapGenerator node.
            mapNodes.Add(newNode);
            AddChild(newNode); 
        }

        // Connect Waypoints based on their distance and terrain type (if implemented).
        foreach (var node in mapNodes)
        {
            // Find nearby nodes within a radius (currently 1/5th of the map size).
            List<Waypoint> nearbyNodes = GetNearbyNodes(node, mapNodes, mapSize / 5); 

            // Attempt to connect the current node to each nearby node.
            foreach (var nearbyNode in nearbyNodes)
            {
                if (CanConnectNodes(node, nearbyNode))
                {
                    // Connect the nodes (the actual connection logic is in the Waypoint class).
                    node.ConnectTo(nearbyNode); 
                }
            }
        }

        GD.Print("Map generation complete.");
        return mapNodes;
    }

    // --- The methods below are placeholders and need to be implemented based on your game design --- 

    /// <summary>
    /// (Not Implemented) Generates an array of TerrainFeatures to be used for the map.
    /// </summary>
    /// <param name="mapSize">The size of the map.</param>
    /// <returns>An array of TerrainFeature objects (not implemented yet).</returns>
    private TerrainFeature[] GenerateTerrainFeatures(int mapSize)
    {
        // TODO: Implement terrain feature generation logic here
        // Example: You can use Perlin noise or other algorithms for terrain generation.
        // You can define different terrain types (mountains, forests, plains) and assign them
        // based on the generated noise values.
        throw new NotImplementedException();
    }

    /// <summary>
    /// (Not Implemented) Determines the TerrainFeature at a given position on the map.
    /// </summary>
    /// <param name="position">The 2D position to check.</param>
    /// <param name="terrainFeatures">The array of generated TerrainFeatures.</param>
    /// <returns>The TerrainFeature at the specified position (not implemented yet).</returns>
    private TerrainFeature GetTerrainFeatureAtPosition(Vector2 position, TerrainFeature[] terrainFeatures)
    {
        // TODO: Implement logic to determine terrain feature at a position.
        // Example: Use position coordinates to index into the terrainFeatures array.
        throw new NotImplementedException();
    }

    /// <summary>
    /// Finds and returns a list of Waypoints within a certain radius of the given node.
    /// </summary>
