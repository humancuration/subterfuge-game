using System;
using System.Collections.Generic;
using Systems;
using Models;
using UI;
using Godot;

namespace Models
{
    public class GameManager : Node, IUpdatable
    {
        // Systems
        private AISystem aiSystem;
        private NodeSystem nodeSystem;
        private DialogueManager dialogueManager;
        private EconomySystem economySystem;
        private HealthSystem healthSystem;
        private TechnologySystem technologySystem;
        private QuestSystem questSystem;
        private FactionSystem factionSystem;
        private ReputationSystem reputationSystem;
        private TimeSystem timeSystem;
        private EnvironmentManager environmentManager;
        private ComplexEventSystem complexEventSystem;
        private PopulationSystem populationSystem;
        private List<IUpdatable> systems;

        // Game entities
        public List<NPC> NPCs { get; private set; }
        public List<Node> Nodes { get; private set; }

        // Events
        public event Action<float> OnUpdate;

        // UI References
        private EventUI eventUI;

        public GameManager(List<NPC> npcs, List<Node> nodes)
        {
            NPCs = npcs;
            Nodes = nodes;
        }

        public override void _Ready()
        {
            base._Ready();
            InitializeSystems();
            // Optionally trigger initial population
            populationSystem.PopulateMultipleAreas();
        }

        private void InitializeSystems()
        {
            aiSystem = new AISystem(NPCs);
            nodeSystem = new NodeSystem(Nodes);
            economySystem = new EconomySystem(Nodes);
            healthSystem = new HealthSystem(Nodes);
            technologySystem = new TechnologySystem(Nodes);
            dialogueManager = new DialogueManager(this);
            questSystem = new QuestSystem();
            factionSystem = new FactionSystem();
            reputationSystem = new ReputationSystem();
            timeSystem = new TimeSystem();
            environmentManager = new EnvironmentManager();
            complexEventSystem = new ComplexEventSystem(this);
            populationSystem = new PopulationSystem(this);

            systems = new List<IUpdatable> 
            { 
                aiSystem, nodeSystem, economySystem, healthSystem, 
                technologySystem, dialogueManager, questSystem, 
                factionSystem, reputationSystem, timeSystem, 
                environmentManager, complexEventSystem, populationSystem 
            };

            // Add child nodes
            foreach (var system in systems)
            {
                if (system is Node nodeSystem)
                {
                    AddChild(nodeSystem);
                }
            }

            // Connect ComplexEventSystem to EventUI
            eventUI = GetEventUI();
            if (eventUI != null)
            {
                complexEventSystem.ConnectEventNotifier(eventUI);
            }
            else
            {
                GD.PrintErr("Warning: EventUI not found. Complex events may not be displayed properly.");
            }
        }

        private EventUI GetEventUI()
        {
            return GetNode<EventUI>("Path/To/EventUI");
        }

        public override void _Process(float delta)
        {
            Update(delta);
        }

        public void Update(float deltaTime)
        {
            foreach (var system in systems)
            {
                system.Update(deltaTime);
            }

            OnUpdate?.Invoke(deltaTime);
        }

        // System getters
        public DialogueManager GetDialogueManager() => dialogueManager;
        public EconomySystem GetEconomySystem() => economySystem;
        public HealthSystem GetHealthSystem() => healthSystem;
        public TechnologySystem GetTechnologySystem() => technologySystem;
        public QuestSystem GetQuestSystem() => questSystem;
        public FactionSystem GetFactionSystem() => factionSystem;
        public ReputationSystem GetReputationSystem() => reputationSystem;
        public TimeSystem GetTimeSystem() => timeSystem;
        public EnvironmentManager GetEnvironmentManager() => environmentManager;
        public ComplexEventSystem GetComplexEventSystem() => complexEventSystem;
        public PopulationSystem GetPopulationSystem() => populationSystem;

        // Entity management methods
        public void AddNPC(NPC npc) => NPCs.Add(npc);
        public void RemoveNPC(NPC npc) => NPCs.Remove(npc);
        public void AddNode(Node node) => Nodes.Add(node);
        public void RemoveNode(Node node) => Nodes.Remove(node);

        // Game state management methods
        public void SaveGame(string filePath)
        {
            // TODO: Implement save game logic
            // This should serialize all relevant game state, including:
            // - All system states
            // - NPC and Node data
            // - Player progress and stats
            throw new NotImplementedException("SaveGame method needs to be implemented.");
        }

        public void LoadGame(string filePath)
        {
            // TODO: Implement load game logic
            // This should deserialize the game state and:
            // - Restore all system states
            // - Recreate NPCs and Nodes
            // - Set player progress and stats
            throw new NotImplementedException("LoadGame method needs to be implemented.");
        }
    }
}

// sonnet revision
// These consolidated versions include the following enhancements:
// 1. GameManager:
// Added new systems: QuestSystem, FactionSystem, ReputationSystem, and TimeSystem.
//  Included an OnUpdate event for better integration with the UI.
// Added methods to add/remove NPCs and Nodes dynamically.
// Included placeholder methods for saving and loading games.

// o1 mini revision
// EnvironmentManager and ComplexEventSystem are initialized and added to the list of systems.
// GetComplexEventSystem provides access to the ComplexEventSystem for other parts of the game.
// The Update method now updates the ComplexEventSystem alongside other systems.

// sonnet revision
// This improved version of the GameManager class incorporates the following enhancements:

// All systems from both versions are now included and initialized.
// The ComplexEventSystem is connected to the EventUI for better event handling and display.
// A new GetEventUI method is added as a placeholder for retrieving the EventUI instance. You'll need to implement this based on your specific UI framework (Unity, Godot, etc.).
// Error handling is added in case the EventUI is not found.
// The SaveGame and LoadGame methods are included as placeholders with TODO comments for future implementation.
// The class structure is organized for better readability, with system initializations, getters, entity management, and game state management methods grouped together.

//To fully implement this class, you'll need to:

// Replace the GetEventUI method with the appropriate code for your UI framework.
// Implement the SaveGame and LoadGame methods to handle game state serialization and deserialization.
// Ensure that all the system classes (AISystem, NodeSystem, etc.) are properly implemented and follow the IUpdatable interface.

// This improved GameManager class provides a solid foundation for managing the various systems and entities in your game, with improved event handling and preparation for save/load functionality.

/* Improvements and changes:
The class now inherits from Node and implements IUpdatable, combining both approaches.
Moved the InitializeSystems() call to the _Ready() method, which is the proper place for initialization in Godot.
Added the PopulationSystem to the list of systems and provided a getter for it.
Implemented the _Process(float delta) method, which calls Update(delta). This ensures the game loop is properly integrated with Godot's engine cycle.
Changed Console.WriteLine to GD.PrintErr for error logging, which is more appropriate for Godot.
Updated the GetEventUI() method to use Godot's GetNode<T>() method. You'll need to adjust the path to match your scene structure.
Added a loop in InitializeSystems() to add all systems that are Node instances as children of the GameManager. This ensures proper integration with Godot's scene tree.
Removed comments that were no longer relevant after the merge.
This combined version maintains the functionality of both original versions while better integrating with Godot's architecture. The GameManager now acts as a central hub for all game systems, properly initialized and updated within Godot's lifecycle.
To fully implement this class, you'll need to:
Ensure all system classes are properly implemented and follow both the IUpdatable interface and inherit from Node where appropriate.
Implement the SaveGame and LoadGame methods for game state serialization and deserialization.
Adjust the path in GetEventUI() to match your actual scene structure.
Implement any additional methods or properties specific to your game's needs.
This structure provides a solid foundation for managing the various systems and entities in your game, with improved integration with Godot and preparation for save/load functionality.*/