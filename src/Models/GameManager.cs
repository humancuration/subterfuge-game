using System;
using System.Collections.Generic;
using Systems;
using Models;

namespace Models
{
    public class GameManager : IUpdatable
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
        private List<IUpdatable> systems;

        // Game entities
        public List<NPC> NPCs { get; private set; }
        public List<Node> Nodes { get; private set; }

        // Events
        public event Action<float> OnUpdate;

        public GameManager(List<NPC> npcs, List<Node> nodes)
        {
            NPCs = npcs;
            Nodes = nodes;

            InitializeSystems();
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

            systems = new List<IUpdatable> 
            { 
                aiSystem, nodeSystem, economySystem, healthSystem, 
                technologySystem, dialogueManager, questSystem, 
                factionSystem, reputationSystem, timeSystem 
            };
        }

        public void Update(float deltaTime)
        {
            foreach (var system in systems)
            {
                system.Update(deltaTime);
            }

            OnUpdate?.Invoke(deltaTime);
        }

        public DialogueManager GetDialogueManager() => dialogueManager;
        public EconomySystem GetEconomySystem() => economySystem;
        public HealthSystem GetHealthSystem() => healthSystem;
        public TechnologySystem GetTechnologySystem() => technologySystem;
        public QuestSystem GetQuestSystem() => questSystem;
        public FactionSystem GetFactionSystem() => factionSystem;
        public ReputationSystem GetReputationSystem() => reputationSystem;
        public TimeSystem GetTimeSystem() => timeSystem;

        public void AddNPC(NPC npc) => NPCs.Add(npc);
        public void RemoveNPC(NPC npc) => NPCs.Remove(npc);
        public void AddNode(Node node) => Nodes.Add(node);
        public void RemoveNode(Node node) => Nodes.Remove(node);

        public void SaveGame(string filePath)
        {
            // Implement save game logic
        }

        public void LoadGame(string filePath)
        {
            // Implement load game logic
        }
    }
}

// These consolidated versions include the following enhancements:
// 1. GameManager:
// Added new systems: QuestSystem, FactionSystem, ReputationSystem, and TimeSystem.
//  Included an OnUpdate event for better integration with the UI.
// Added methods to add/remove NPCs and Nodes dynamically.
// Included placeholder methods for saving and loading games.