using Godot;
using System;
using System.Collections.Generic;
using Models;
using Systems;

namespace Models
{
    public class GameManager : IUpdatable
    {
        public Character PlayerCharacter { get; private set; }
        public List<NPC> NPCs { get; private set; }
        public List<Node> Nodes { get; private set; }
        public DialogueManager DialogueManager { get; private set; }

        public event Action<float> OnUpdate;

        public GameManager(List<NPC> npcs, List<Node> nodes)
        {
            NPCs = npcs;
            Nodes = nodes;
            DialogueManager = new DialogueManager();

            // Initialize player character
            PlayerCharacter = new Character("Player", "DefaultAppearance"); // Replace with actual appearance options
        }

        public void Update(float deltaTime)
        {
            // Update game systems
            DialogueManager.Update(deltaTime);

            // Trigger OnUpdate event
            OnUpdate?.Invoke(deltaTime);
        }

        public DialogueManager GetDialogueManager()
        {
            return DialogueManager;
        }
    }
}