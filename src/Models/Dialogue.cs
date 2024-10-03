using Godot;
using System;
using System.Collections.Generic;

namespace Models
{
    public class Dialogue
    {
        public string Speaker { get; private set; }
        public string Text { get; private set; }
        public List<DialogueOption> Options { get; private set; } = new List<DialogueOption>();

        public Dialogue(string speaker, string text)
        {
            Speaker = speaker;
            Text = text;
        }

        public void AddOption(DialogueOption option)
        {
            Options.Add(option);
        }
    }

    public class DialogueOption
    {
        public string OptionText { get; private set; }
        public Action OnSelect { get; private set; }

        public DialogueOption(string optionText, Action onSelect)
        {
            OptionText = optionText;
            OnSelect = onSelect;
        }
    }
}