// 5. Saving and Loading Game State
// Implementing functionality to save and load the current game state, ensuring continuity between sessions.
// 5.1. GameState Class
// File: Models/GameState.cs


using System;
using System.Collections.Generic;

namespace Models
{
    [Serializable]
    public class GameState
    {
        public List<NPCData> NPCs { get; set; }
        public List<NodeData> Nodes { get; set; }

        public GameState()
        {
            NPCs = new List<NPCData>();
            Nodes = new List<NodeData>();
        }
    }

    [Serializable]
    public class NPCData
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public int CurrentNodeId { get; set; }
        public List<string> Traits { get; set; }

        public NPCData()
        {
            Traits = new List<string>();
        }
    }

    [Serializable]
    public class NodeData
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public NodeStats Stats { get; set; }
        public List<int> ConnectedNodeIds { get; set; }

        public NodeData()
        {
            ConnectedNodeIds = new List<int>();
        }
    }
}

// Explanation:
// GameState Class: Encapsulates the entire state of the game, including all NPCs and Nodes.
// NPCData & NodeData Classes: Serializable representations of NPCs and Nodes for easy saving and loading.