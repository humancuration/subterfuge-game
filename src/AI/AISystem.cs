// 1.6. Triggering a Dialogue
// Let's modify the AISystem to trigger dialogues under certain conditions, such as when an NPC interacts with the player.
// File: src/AI/AISystem.cs

using System;
using System.Collections.Generic;
using Models;

namespace Systems
{
    public class AISystem : IUpdatable
    {
        private List<NPC> npcs;
        private Random rand;

        public AISystem(List<NPC> npcList)
        {
            npcs = new List<NPCController>();
            foreach (var npc in npcList)
            {
                var controller = new NPCController(npc.Name, new AIController());
                npcs.Add(controller);
            }
            rand = new Random();
        }
        
        public void Update(float deltaTime)
        {
            foreach (var npcController in npcs)
            {
                npcController.Update(deltaTime);
                npcController.DecideNextAction();
            }
        }
        

        private bool ShouldStartDialogue(NPC npc)
        {
            // Define your condition to start a dialogue
            // For example, a random chance
            return rand.NextDouble() < 0.01; // 1% chance each update
        }

        private void StartDialogue(NPC npc)
        {
            Dialogue dialogue = ExampleDialogue.CreateConversation();
            Godot.GodotManager.Instance.GetGameManager().GetDialogueManager().StartDialogue(npc, dialogue);
        }
    }
}

// Explanation:
// AISystem Class: Checks each NPC to determine if a dialogue should be initiated based on defined conditions (e.g., random chance).
// StartDialogue Method: Initiates the dialogue using the DialogueManager.