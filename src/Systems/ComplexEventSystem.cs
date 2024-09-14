using Godot;
using Godot.Collections;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using Models;
using Localization;

namespace Systems
{
    public class ComplexEventSystem : Node, IUpdatable
    {   
        public string Id { get; set; }
        public string TitleKey { get; set; }
        public string Description { get; set; }
        private List<ComplexGameEvent> allEvents;
        private List<ComplexGameEvent> activeEvents;
        private GameManager gameManager;
        private Random rnd = new Random();
        private NameDatabase nameDatabase;
        private LocalizationManager localizationManager;

        [Signal]
        public delegate void EventTriggered(ComplexGameEvent gameEvent);

        public ComplexEventSystem(GameManager manager)
        {
            gameManager = manager;
            activeEvents = new List<ComplexGameEvent>();

            // Load name database
            nameDatabase = new NameDatabase("res://src/Data/Names.json");

            // Load localization data
            localizationManager = new LocalizationManager();
            localizationManager.LoadLocalizedText("en", "res://src/Data/LocalizedText_en.json");
            // Load other languages as needed

            LoadEvents("res://src/Data/EventDefinitions.json");
            GD.Print("ComplexEventSystem initialized.");
        }

        private void LoadEvents(string filePath)
        {
            try
            {
                string fullPath = ProjectSettings.GlobalizePath(filePath);
                if (File.Exists(fullPath))
                {
                    string json = File.ReadAllText(fullPath);
                    var eventData = JsonConvert.DeserializeObject<EventDefinitionData>(json);
                    allEvents = eventData.Events;
                    GD.Print("Events loaded successfully.");
                }
                else
                {
                    GD.PrintErr($"Event definitions file not found: {filePath}");
                }
            }
            catch (Exception e)
            {
                GD.PrintErr($"Error loading event definitions: {e.Message}");
            }
        }

        public void Update(float deltaTime)
        {
            if (ShouldTriggerEvent())
            {
                var newEvent = SelectRandomEligibleEvent();
                if (newEvent != null)
                {
                    TriggerEvent(newEvent);
                }
            }

            // Update active events if needed
        }

        private bool ShouldTriggerEvent()
        {
            return rnd.NextDouble() < 0.01; // 1% chance each update
        }

        private ComplexGameEvent SelectRandomEligibleEvent()
        {
            List<ComplexGameEvent> eligibleEvents = new List<ComplexGameEvent>();

            foreach (var gameEvent in allEvents)
            {
                if (gameEvent.Conditions.IsMet(gameManager))
                {
                    eligibleEvents.Add(gameEvent);
                }
            }

            if (eligibleEvents.Count > 0)
            {
                return eligibleEvents[rnd.Next(eligibleEvents.Count)];
            }

            return null;
        }

        private void TriggerEvent(ComplexGameEvent gameEvent)
        {
            activeEvents.Add(gameEvent);

            // Replace placeholders in the event description
            string processedDescription = ProcessEventDescription(gameEvent.Description);

            // Get the localized title using the titleKey
            string localizedTitle = localizationManager.GetText(gameEvent.TitleKey);

            GD.Print($"Event Triggered: {localizedTitle} - {processedDescription}");

            // Emit signal to notify UI
            EmitSignal(nameof(EventTriggered), gameEvent);
        }

        public void HandlePlayerChoice(ComplexGameEvent gameEvent, PlayerChoice choice)
        {
            foreach (var effect in choice.Outcome.Effects)
            {
                ApplyEffect(effect.Key, effect.Value);
            }

            foreach (var nextEventId in choice.Outcome.TriggerNextEvents)
            {
                var nextEvent = allEvents.Find(e => e.Id == nextEventId);
                if (nextEvent != null && nextEvent.Conditions.IsMet(gameManager))
                {
                    TriggerEvent(nextEvent);
                }
            }

            activeEvents.Remove(gameEvent);
            GD.Print($"Player chose: {choice.ChoiceText}");
        }

        private void ApplyEffect(string statName, float value)
        {
            switch (statName.ToLower())
            {
                case "morale":
                    foreach (var node in gameManager.Nodes)
                    {
                        node.Stats.Morale = Math.Clamp(node.Stats.Morale + value, 0f, 100f);
                    }
                    break;
                case "resourceavailability":
                    foreach (var node in gameManager.Nodes)
                    {
                        node.Stats.ResourceAvailability = Math.Clamp(node.Stats.ResourceAvailability + value, 0f, 100f);
                    }
                    break;
                case "economicprosperity":
                    gameManager.GetEconomySystem().EconomicProsperity += value;
                    break;
                case "culturaldevelopment":
                    foreach (var node in gameManager.Nodes)
                    {
                        node.Stats.CulturalDevelopment = Math.Clamp(node.Stats.CulturalDevelopment + value, 0f, 100f);
                    }
                    break;
                default:
                    GD.PrintErr($"Unknown effect stat: {statName}");
                    break;
            }
        }

        public void ConnectEventNotifier(Node target, string method = "DisplayEvent")
        {
            Connect(nameof(EventTriggered), target, method);
        }

        private string ProcessEventDescription(string description)
        {
            return description
                .Replace("{nodeName}", nameDatabase.GetRandomNodeName())
                .Replace("{npcFirstName}", nameDatabase.GetRandomNPCName().Split(' ')[0])
                .Replace("{npcLastName}", nameDatabase.GetRandomNPCName().Split(' ')[1])
                .Replace("{cultureName}", nameDatabase.GetRandomElement(nameDatabase.Data.culture_names));
        }
    }

    public class EventDefinitionData
    {
        [JsonProperty("events")]
        public List<ComplexGameEvent> Events { get; set; }
    }
}

// o1 mini
// LoadEvents: Loads event definitions from EventDefinitions.json using Newtonsoft.Json.
// Update: Periodically checks if an event should be triggered based on random chance and conditions.
// SelectRandomEligibleEvent: Filters events that meet the current game state conditions and selects one at random.
// TriggerEvent: Adds the event to active events and logs it; you can expand this to notify the UI.
// HandlePlayerChoice: Applies the effects of the player's choice and triggers any subsequent events.
// ApplyEffect: Maps stat changes to the game state, modifying various attributes across nodes or systems.

// claude sonnet
// Key changes and reconciliations:
// The class now inherits from Node instead of just implementing IUpdatable. This allows it to use Godot's built-in signal system.
// Removed the separate eventNotifier node. The ComplexEventSystem itself can now emit signals.
// The EventTriggered signal is now defined using Godot's [Signal] attribute.
// The TriggerEvent method now uses EmitSignal directly instead of calling it on a separate node.
// Added a ConnectEventNotifier method that connects the EventTriggered signal to a target node. This replaces the previous Connect call on the eventNotifier.
// Kept all the existing functionality from the first version, including event loading, updating, and handling player choices.
// This reconciled version combines the functionality of both previous versions, providing a robust event system that can trigger events, handle player choices, and notify other parts of the game (like the UI) when events occur. The system is now more integrated with Godot's node and signal system, which should make it easier to use within your Godot project.


// opus rewrite, sonnet merge
// This merged version includes the following enhancements:
// Added NameDatabase and LocalizationManager as private fields.
// Initialized NameDatabase and LocalizationManager in the constructor.
// Modified the TriggerEvent method to process the event description and use localized titles.
// Added the ProcessEventDescription method to replace placeholders with random names.
// Kept all the existing functionality from the previous version, including event loading, updating, and handling player choices.
// Maintained the Godot signal system integration for notifying the UI about triggered events.

