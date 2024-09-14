// File: Models/Quest.cs
// Based on the code we have so far and the goals you've outlined for your game, here are some additional recommendations and code snippets to enhance your game:
// Quest System
// Implement a quest system to give players objectives and guide them through the game's narrative. This can tie into the dialogue system and influence the game's stats and events.



using System.Collections.Generic;

namespace Models
{
    public class Quest
    {
        public string Title { get; set; }
        public string Description { get; set; }
        public List<string> Objectives { get; set; }
        public bool IsCompleted { get; set; }
        public int RewardExperience { get; set; }
        public int RewardGold { get; set; }

        public void CompleteObjective(string objective)
        {
            if (Objectives.Contains(objective))
            {
                Objectives.Remove(objective);
                if (Objectives.Count == 0)
                {
                    IsCompleted = true;
                }
            }
        }
    }
}

// File: Systems/QuestSystem.cs
using System.Collections.Generic;
using Models;

namespace Systems
{
    public class QuestSystem : IUpdatable
    {
        private List<Quest> activeQuests;
        private List<Quest> completedQuests;

        public QuestSystem()
        {
            activeQuests = new List<Quest>();
            completedQuests = new List<Quest>();
        }

        public void Update(float deltaTime)
        {
            // Check quest objectives and update quest states
            foreach (var quest in activeQuests)
            {
                if (quest.IsCompleted)
                {
                    CompleteQuest(quest);
                }
            }
        }

        public void StartQuest(Quest quest)
        {
            activeQuests.Add(quest);
        }

        private void CompleteQuest(Quest quest)
        {
            activeQuests.Remove(quest);
            completedQuests.Add(quest);
            // Reward player with experience and gold
            // Update relevant game stats and trigger events
        }
    }
}