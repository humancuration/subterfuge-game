// src/Characters/DialogueManager.cs

using System;
using System.Collections.Generic;

namespace Systems
{
    public class DialogueManager : IUpdatable
    {
        private Queue<Dialogue> dialogueQueue;
        private bool isDialogueActive;
        private NPC currentNPC;
        private GameManager gameManager;
        
        public bool IsAutopilotEnabled { get; set; }
        public float AutoplayDelay { get; set; } = 2f; // Seconds to wait before auto-selecting an option
        private float autoplayTimer;

        public event Action<Dialogue> OnDialogueStarted;
        public event Action<DialogueOption> OnOptionSelected;
        public event Action OnDialogueEnded;

        public DialogueManager(GameManager manager)
        {
            dialogueQueue = new Queue<Dialogue>();
            isDialogueActive = false;
            gameManager = manager;
        }

        public void StartDialogue(NPC npc, Dialogue dialogue)
        {
            currentNPC = npc;
            dialogueQueue.Enqueue(dialogue);
            isDialogueActive = true;
            OnDialogueStarted?.Invoke(dialogue);
            DisplayNextDialogue();
        }

        public void Update(float deltaTime)
        {
            if (isDialogueActive && dialogueQueue.Count > 0)
            {
                if (IsAutopilotEnabled)
                {
                    autoplayTimer += deltaTime;
                    if (autoplayTimer >= AutoplayDelay)
                    {
                        AutoplayDialogue();
                        autoplayTimer = 0f;
                    }
                }
                // Handle timed dialogues or other time-based events here
            }
        }

        private void DisplayNextDialogue()
        {
            if (dialogueQueue.Count == 0)
            {
                EndDialogue();
                return;
            }

            var dialogue = dialogueQueue.Peek(); // Peek instead of Dequeue
            // Interface with Godot to display dialogue UI
            GodotManager.Instance.DisplayDialogue(dialogue);
            autoplayTimer = 0f; // Reset autoplay timer
        }

        public void SelectOption(int optionIndex)
        {
            if (isDialogueActive && dialogueQueue.Count > 0)
            {
                var currentDialogue = dialogueQueue.Dequeue();
                if (optionIndex < currentDialogue.Options.Count)
                {
                    var selectedOption = currentDialogue.Options[optionIndex];
                    selectedOption.OnSelect?.Invoke(currentNPC, gameManager);
                    OnOptionSelected?.Invoke(selectedOption);
                    DisplayNextDialogue();
                }
            }
        }

        private void AutoplayDialogue()
        {
            if (isDialogueActive && dialogueQueue.Count > 0)
            {
                var currentDialogue = dialogueQueue.Peek();
                // Select a random option or the first one
                int optionIndex = new Random().Next(currentDialogue.Options.Count);
                SelectOption(optionIndex);
            }
        }

        private void EndDialogue()
        {
            isDialogueActive = false;
            currentNPC = null;
            OnDialogueEnded?.Invoke();
        }

        public void AddDialogueToQueue(Dialogue dialogue)
        {
            dialogueQueue.Enqueue(dialogue);
            if (!isDialogueActive)
            {
                DisplayNextDialogue();
            }
        }

        public void ClearDialogueQueue()
        {
            dialogueQueue.Clear();
            EndDialogue();
        }

        public bool IsInDialogue()
        {
            return isDialogueActive;
        }
    }

    public class Dialogue
    {
        public string Speaker { get; set; }
        public string Text { get; set; }
        public List<DialogueOption> Options { get; set; }

        public Dialogue(string speaker, string text)
        {
            Speaker = speaker;
            Text = text;
            Options = new List<DialogueOption>();
        }

        public void AddOption(string text, Action<NPC, GameManager> onSelect)
        {
            Options.Add(new DialogueOption(text, onSelect));
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

// Here are the changes and additions I've made:
// Autopilot Feature: Incorporated the IsAutopilotEnabled property and added an AutoplayDialogue method.
// Autoplay Delay: Added an AutoplayDelay property to control how long the autopilot waits before selecting an option.
// Events: Added OnDialogueStarted, OnOptionSelected, and OnDialogueEnded events for better integration with other systems.
// Dialogue Peeking: Changed dialogueQueue.Dequeue() to dialogueQueue.Peek() in DisplayNextDialogue to allow for revisiting dialogue options.
// Dialogue and DialogueOption Classes: Included these classes in the same file for easier reference.
// Additional Methods:
// AddDialogueToQueue: Allows adding new dialogues while a conversation is ongoing.
// ClearDialogueQueue: Clears all pending dialogues and ends the current conversation.
// IsInDialogue: Checks if a dialogue is currently active.
// EndDialogue Method: Added a separate method to handle dialogue ending, including event triggering.
// Random Option Selection: In autopilot mode, a random option is selected instead of always choosing the first one.
// This combined version provides a more robust dialogue system with autopilot capabilities, better event handling, and additional utility methods. It should integrate well with your existing systems while providing more flexibility for future expansions.