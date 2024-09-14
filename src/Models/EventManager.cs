// 4.2. Event Manager Enhancements
// File: Models/EventManager.cs

using System;

namespace Models
{
    public static class EventManager
    {
        public static event Action<GameEvent> OnEventTriggered;

        public static void TriggerEvent(GameEvent gameEvent)
        {
            OnEventTriggered?.Invoke(gameEvent);
            gameEvent.Trigger();
        }
    }
}

// Explanation:
// EventManager Class: Remains responsible for managing and triggering events. The console logs help in debugging and tracking event flow.