// File: src/SaveLoad/SaveLoadManager.cs
using Godot;
using System.IO;
using Newtonsoft.Json;

public class SaveLoadManager : Node
{
    public void SaveGame(GameState gameState, string filePath)
    {
        string json = JsonConvert.SerializeObject(gameState, Formatting.Indented);
        File.WriteAllText(ProjectSettings.GlobalizePath(filePath), json);
        GD.Print("Game saved successfully.");
    }

    public GameState LoadGame(string filePath)
    {
        if (File.Exists(ProjectSettings.GlobalizePath(filePath)))
        {
            string json = File.ReadAllText(ProjectSettings.GlobalizePath(filePath));
            GameState gameState = JsonConvert.DeserializeObject<GameState>(json);
            GD.Print("Game loaded successfully.");
            return gameState;
        }
        GD.Print("Save file not found.");
        return null;
    }
}

// File: src/SaveLoad/GameState.cs
using System.Collections.Generic;

public class GameState
{
    public int PlayerGold { get; set; }
    public int PlayerFood { get; set; }
    public List<string> PlayerSkills { get; set; }
    // Add other relevant game state information
}