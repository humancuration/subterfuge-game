
// File: Systems/TimeSystem.cs
using Models;

namespace Systems
{
    public class TimeSystem : IUpdatable
    {
        private DateTime gameDateTime;
        private float timeScale;

        public TimeSystem()
        {
            gameDateTime = new DateTime(1, 1, 1, 6f);
            timeScale = 1f;
        }

        public void Update(float deltaTime)
        {
            gameDateTime.AdvanceTime(deltaTime * timeScale);
            // Update game world based on time (e.g., NPC schedules, events)
        }

        public void SetTimeScale(float scale)
        {
            timeScale = scale;
        }

        public DateTime GetCurrentDateTime()
        {
            return gameDateTime;
        }
    }
}