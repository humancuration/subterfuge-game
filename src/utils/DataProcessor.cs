using System;
using System.Collections.Generic;

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

        var processedData = new Dictionary<string, Dictionary<string, double>>
        {
            { "species", speciesList.ToDictionary(sp => sp, sp => 0.0) }, // Placeholder
            { "distance_matrix", distanceMatrix }
        };

        return distanceMatrix;
    }
}