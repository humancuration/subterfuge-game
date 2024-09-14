using Godot;
using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using Models;
using Systems;

public class SaveLoadManager : Node
{
    private const string DefaultSaveFileName = "savegame.json";

    public void SaveGame(GameManager gameManager, string filePath = null)
    {
        if (string.IsNullOrEmpty(filePath))
        {
            filePath = DefaultSaveFileName;
        }

        GameState state = CreateGameState(gameManager);
        string json = JsonConvert.SerializeObject(state, Formatting.Indented);
        File.WriteAllText(ProjectSettings.GlobalizePath(filePath), json);
        GD.Print($"Game saved successfully to {filePath}.");
    }

    public bool LoadGame(GameManager gameManager, string filePath = null)
    {
        if (string.IsNullOrEmpty(filePath))
        {
            filePath = DefaultSaveFileName;
        }

        string fullPath = ProjectSettings.GlobalizePath(filePath);
        if (!File.Exists(fullPath))
        {
            GD.Print($"Save file not found: {filePath}");
            return false;
        }

        try
        {
            string json = File.ReadAllText(fullPath);
            GameState state = JsonConvert.DeserializeObject<GameState>(json);
            ApplyGameState(gameManager, state);
            GD.Print($"Game loaded successfully from {filePath}.");
            return true;
        }
        catch (Exception e)
        {
            GD.PrintErr($"Error loading game: {e.Message}");
            return false;
        }
    }

    private GameState CreateGameState(GameManager gameManager)
    {
        GameState state = new GameState
        {
            PlayerGold = gameManager.PlayerGold,
            PlayerFood = gameManager.PlayerFood,
            PlayerSkills = new List<string>(gameManager.PlayerSkills),
            NPCs = new List<NPCData>(),
            Nodes = new List<NodeData>()
        };

        foreach (var npc in gameManager.NPCs)
        {
            state.NPCs.Add(new NPCData
            {
                Id = npc.Id,
                Name = npc.Name,
                CurrentNodeId = npc.CurrentNode.Id,
                Traits = npc.Traits.ConvertAll(t => t.TraitName)
            });
        }

        foreach (var node in gameManager.Nodes)
        {
            state.Nodes.Add(new NodeData
            {
                Id = node.Id,
                Name = node.NodeName,
                Stats = node.Stats,
                ConnectedNodeIds = node.ConnectedNodes.ConvertAll(n => n.Id),
                IsVisited = node.IsVisited,
                UpgradeLevel = node.UpgradeLevel
            });
        }

        return state;
    }

    private void ApplyGameState(GameManager gameManager, GameState state)
    {
        gameManager.PlayerGold = state.PlayerGold;
        gameManager.PlayerFood = state.PlayerFood;
        gameManager.PlayerSkills = new List<string>(state.PlayerSkills);

        gameManager.NPCs.Clear();
        gameManager.Nodes.Clear();

        Dictionary<int, Node> nodeDictionary = new Dictionary<int, Node>();

        foreach (var nodeData in state.Nodes)
        {
            Node node = new Node(nodeData.Id, nodeData.Name)
            {
                Stats = nodeData.Stats,
                IsVisited = nodeData.IsVisited,
                UpgradeLevel = nodeData.UpgradeLevel
            };
            gameManager.Nodes.Add(node);
            nodeDictionary[node.Id] = node;
        }

        foreach (var nodeData in state.Nodes)
        {
            Node currentNode = nodeDictionary[nodeData.Id];
            foreach (var connectedId in nodeData.ConnectedNodeIds)
            {
                if (nodeDictionary.TryGetValue(connectedId, out Node connectedNode))
                {
                    currentNode.ConnectNode(connectedNode);
                }
            }
        }

        foreach (var npcData in state.NPCs)
        {
            if (nodeDictionary.TryGetValue(npcData.CurrentNodeId, out Node currentNode))
            {
                NPC npc = new NPC(npcData.Id, npcData.Name, currentNode);
                foreach (var traitName in npcData.Traits)
                {
                    ITrait trait = TraitFactory.CreateTrait(traitName);
                    if (trait != null)
                    {
                        npc.Traits.Add(trait);
                    }
                }
                gameManager.NPCs.Add(npc);
            }
        }
    }
}

public class GameState
{
    public int PlayerGold { get; set; }
    public int PlayerFood { get; set; }
    public List<string> PlayerSkills { get; set; }
    public List<NPCData> NPCs { get; set; }
    public List<NodeData> Nodes { get; set; }
}

public class NPCData
{
    public int Id { get; set; }
    public string Name { get; set; }
    public int CurrentNodeId { get; set; }
    public List<string> Traits { get; set; }
}

public class NodeData
{
    public int Id { get; set; }
    public string Name { get; set; }
    public NodeStats Stats { get; set; }
    public List<int> ConnectedNodeIds { get; set; }
    public bool IsVisited { get; set; }
    public int UpgradeLevel { get; set; }
}

public static class TraitFactory
{
    public static ITrait CreateTrait(string traitName)
    {
        switch (traitName)
        {
            case "Optimist":
                return new OptimistTrait();
            case "Saboteur":
                return new SaboteurTrait();
            // Add other traits as needed
            default:
                GD.PrintErr($"Unknown trait: {traitName}");
                return null;
        }
    }
}

// This improved version:
// 1. Combines the Godot-specific approach with the more detailed serialization from the first version.
// Uses a default file name if none is provided.
// Includes error handling for the loading process.
// 4. Separates the logic for creating and applying game states into separate methods for better organization.
// Includes all the necessary data classes (GameState, NPCData, NodeData) in the same file for easier reference.
// Retains the TraitFactory for recreating trait instances.
// Uses Godot's GD.Print for logging instead of Console.WriteLine for better integration with Godot's debugging tools.
// Includes additional Node properties like IsVisited and UpgradeLevel in the saved data.
// To use this SaveLoadManager in your game:
// Attach this script to a Node in your Godot scene.
// Call SaveGame and LoadGame methods from your game logic, passing the GameManager instance.
// Ensure that your GameManager class has the properties and methods referenced in this script (e.g., PlayerGold, PlayerFood, PlayerSkills, NPCs, Nodes).
// Implement the ITrait interface and the specific trait classes (like OptimistTrait and SaboteurTrait).
// This implementation provides a robust system for saving and loading your game state, including detailed information about NPCs, Nodes, and their relationships. It's flexible enough to handle future additions to your game state and integrates well with Godot's systems.