// File: src/Data/DataManager.cs
using Godot;
using System.Collections.Generic;
using Newtonsoft.Json;
using System.IO;

public class DataManager : Node
{
    public Dictionary<string, object> GameData { get; private set; } = new Dictionary<string, object>();

    public override void _Ready()
    {
        LoadData("res://data/base_game_data.json");
    }

    public void LoadData(string filePath)
    {
        string json = File.ReadAllText(ProjectSettings.GlobalizePath(filePath));
        var data = JsonConvert.DeserializeObject<Dictionary<string, object>>(json);
        foreach (var kvp in data)
        {
            GameData[kvp.Key] = kvp.Value;
        }
        GD.Print($"Loaded data from {filePath}");
    }

    public void AddData(string filePath)
    {
        LoadData(filePath);
    }
}