using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

public class DataProcessor
{
    public Dictionary<string, Dictionary<string, double>> ProcessData(List<Dictionary<string, string>> rawData)
    {
        // Example processing similar to data_processor.py
        // Assuming rawData contains "species" and "genetic_marker" keys

        var df = new List<Dictionary<string, string>>(rawData); // Simple representation

        var species = new HashSet<string>();
        foreach (var entry in df)
        {
            species.Add(entry["species"]);
        }

        var speciesList = species.ToList();
        var distanceMatrix = new Dictionary<string, Dictionary<string, double>>();

        foreach (var sp1 in speciesList)
        {
            distanceMatrix[sp1] = new Dictionary<string, double>();
            var markers1 = df.FindAll(entry => entry["species"] == sp1).Select(entry => entry["genetic_marker"]).ToList();

            foreach (var sp2 in speciesList)
            {
                var markers2 = df.FindAll(entry => entry["species"] == sp2).Select(entry => entry["genetic_marker"]).ToList();
                double distance = MathUtils.CalculateGeneticDistance(markers1, markers2);
                distanceMatrix[sp1][sp2] = distance;
            }
        }

        // Example: Assign random positions for visualization purposes
        var visualPositions = GenerateRandomPositions(speciesList.Count);
        var visualPositionDict = speciesList.Zip(visualPositions, (sp, pos) => new { sp, pos })
                                            .ToDictionary(x => x.sp, x => x.pos);

        // Store positions in GameManager for visualization
        var gameManager = GetGameManager();
        if (gameManager != null)
        {
            gameManager.VisualPositions = visualPositionDict;
        }

        return distanceMatrix;
    }

    // ... existing code ...
    public class NodeStats
    {
    // ... existing stats ...
        public float MedicalResources { get; set; }

    // New Stat
        public Dictionary<string, float> Reputation { get; set; }

        public NodeStats()
        {
            Reputation = new Dictionary<string, float>
            {
                { "Player", 50f } // Initialize with neutral reputation
            };
        }

    // ... existing methods ...
    }

    private List<Vector3> GenerateRandomPositions(int count)
    {
        var rand = new Random();
        var positions = new List<Vector3>();
        for (int i = 0; i < count; i++)
        {
            positions.Add(new Vector3((float)(rand.NextDouble() * 100 - 50),
                                      (float)(rand.NextDouble() * 100 - 50),
                                      (float)(rand.NextDouble() * 100 - 50)));
        }
        return positions;
    }

    private GameManager GetGameManager()
    {
        return (GameManager)Godot.GodotManager.Instance.GetNode("/root/GameManager");
    }
}