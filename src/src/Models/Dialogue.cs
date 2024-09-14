// 1. Dialogue System
// The Dialogue System allows for multi-layer conversations between the player and NPCs, enabling dynamic interactions that can influence game stats and narrative.

// 1.1. Dialogue Classes
// File: Models/Dialogue.cs

using System;
using System.Collections.Generic;

namespace Models
{
    public class Dialogue
    {
        public string Speaker { get; private set; }
        public string Text { get; private set; }
        public List<DialogueOption> Options { get; private set; }

        public Dialogue(string speaker, string text)
        {
            Speaker = speaker;
            Text = text;
            Options = new List<DialogueOption>();
        }

        public void AddOption(DialogueOption option)
        {
            Options.Add(option);
        }
    }

    public class DialogueOption
    {
        public string OptionText { get; private set; }
        public Action<NPC, GameManager> OnSelect { get; private set; }

        public DialogueOption(string text, Action<NPC, GameManager> action)
        {
            OptionText = text;
            OnSelect = action;
        }
    }
}

// Explanation:
// Dialogue Class: Represents a single piece of dialogue with a speaker, text, and a list of options the player can choose from.
// DialogueOption Class: Represents an option within a dialogue that the player can select, triggering specific actions affecting NPCs or the game state.