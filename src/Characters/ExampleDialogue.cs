// 1.5. Example Dialogue Implementation
// Let's create an example where the player interacts with an NPC, leading to changes in game stats based on dialogue choices.
// File: Examples/ExampleDialogue.cs

using System;
using Models;

public class ExampleDialogue
{
    public static Dialogue CreateConversation()
    {
        Dialogue dialogue = new Dialogue("Guard", "Halt! Who goes there?");

        DialogueOption option1 = new DialogueOption("I'm a traveler seeking passage.", (npc, gameManager) =>
        {
            // Increase NPC's Morale
            npc.CurrentNode.Stats.Morale += 5f;
            Console.WriteLine("Guard: Very well, you may pass.");
        });

        DialogueOption option2 = new DialogueOption("None of your business!", (npc, gameManager) =>
        {
            // Decrease NPC's Morale
            npc.CurrentNode.Stats.Morale -= 5f;
            // Potentially trigger a negative event
            GameEvent rebellionEvent = new GameEvent("Rebellion Sparked", () =>
            {
                // Effect on nodes
                foreach (var node in gameManager.Nodes)
                {
                    node.Stats.Radicalization += 10f;
                }
                Console.WriteLine("A rebellion has been sparked due to the guard's discontent!");
            });
            EventManager.TriggerEvent(rebellionEvent);
            Console.WriteLine("Guard: That's not very friendly...");
        });

        dialogue.AddOption(option1);
        dialogue.AddOption(option2);

        return dialogue;
    }
}

// Explanation:
// ExampleDialogue Class: Demonstrates how to create a conversation with an NPC where player choices affect game stats and can trigger events.