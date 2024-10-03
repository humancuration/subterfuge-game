using System;
using System.Collections.Generic;

namespace Models
{
    public class Achievement
    {
        public string Name { get; private set; }
        public string Description { get; private set; }
        public bool IsUnlocked { get; set; }
        public Func<GameManager, bool> UnlockCondition { get; private set; }

        public Achievement(string name, string description, Func<GameManager, bool> unlockCondition)
        {
            Name = name;
            Description = description;
            UnlockCondition = unlockCondition;
            IsUnlocked = false;
        }

        public void CheckUnlock(GameManager gameManager)
        {
            if (!IsUnlocked && UnlockCondition(gameManager))
            {
                IsUnlocked = true;
                // Trigger achievement unlocked event or notification
            }
        }
    }
}