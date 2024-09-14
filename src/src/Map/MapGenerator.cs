using Godot;
using System;
using System.Collections.Generic;

public class MapGenerator : Node
{
    private Random rnd = new Random();

    public List<Node> GenerateMap(int numberOfNodes, int mapSize)
    {
        List<Node> mapNodes = new List<Node>();

        // Generate terrain features
        TerrainFeature[] terrainFeatures = GenerateTerrainFeatures(mapSize);

        for (int i = 0; i < numberOfNodes; i++)
        {
            Vector2 position = new Vector2(rnd.Next(-mapSize / 2, mapSize / 2), rnd.Next(-mapSize / 2, mapSize / 2));
            TerrainFeature terrainFeature = GetTerrainFeatureAtPosition(position, terrainFeatures);

            Node newNode = new Node(i, $"Node_{i}", position, terrainFeature);
            mapNodes.Add(newNode);
            AddChild(newNode);
        }

        // Connect nodes based on distance and terrain
        foreach (var node in mapNodes)
        {
            List<Node> nearbyNodes = GetNearbyNodes(node, mapNodes, mapSize / 5);
            foreach (var nearbyNode in nearbyNodes)
            {
                if (CanConnectNodes(node, nearbyNode))
                {
                    node.ConnectNode(nearbyNode);
                }
            }
        }

        GD.Print("Map generation complete.");
        return mapNodes;
    }

    private TerrainFeature[] GenerateTerrainFeatures(int mapSize)
    {
        // Implement terrain feature generation logic here
        // Example: Perlin noise-based terrain generation
        // You can define different terrain types (e.g., mountains, forests, plains)
        // and assign them based on the generated noise values
        // Return an array of TerrainFeature objects
    }

    private TerrainFeature GetTerrainFeatureAtPosition(Vector2 position, TerrainFeature[] terrainFeatures)
    {
        // Implement logic to determine the terrain feature at a given position
        // Example: Use the position coordinates to index into the terrainFeatures array
        // Return the TerrainFeature object at that position
    }

    private List<Node> GetNearbyNodes(Node node, List<Node> allNodes, int radius)
    {
        // Implement logic to find nearby nodes within a given radius
        // Example: Use Vector2.DistanceTo to calculate distances between nodes
        // Return a list of nearby nodes
    }

    private bool CanConnectNodes(Node node1, Node node2)
    {
        // Implement logic to determine if two nodes can be connected
        // Example: Check terrain compatibility, distance, or other factors
        // Return true if the nodes can be connected, false otherwise
    }
}