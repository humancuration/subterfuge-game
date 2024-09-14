// In QuestSystem.cs
using Models;
using Systems;

namespace Systems
{
    public class QuestSystem : IUpdatable
    {
        // Existing QuestSystem code...

        public void CompleteQuest(Quest quest)
        {
            // Existing quest completion logic...

            // Example: Completing a specific quest triggers an event
            if (quest.Id == "quest_save_village")
            {
                var complexEventSystem = gameManager.GetComplexEventSystem();
                var eventToTrigger = complexEventSystem.GetEligibleEvents().Find(e => e.Id == "event_meteor_shower");
                if (eventToTrigger != null)
                {
                    complexEventSystem.TriggerEvent(eventToTrigger);
                }
            }
        }

        // Rest of QuestSystem...
    }
}

// o1 mini
// Explanation
// After completing a specific quest, the QuestSystem accesses the ComplexEventSystem to trigger a relevant event.