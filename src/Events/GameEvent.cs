// 4. Event System Enhancements
// Enhancing the Event System to handle more dynamic and interconnected events.
// 4.1. Enhancing GameEvent Class
// File: Models/GameEvent.cs

using System;

namespace Models
{
    public class GameEvent
    {
        public string EventName { get; private set; }
        public Action Effect { get; private set; }

        public GameEvent(string name, Action effect)
        {
            EventName = name;
            Effect = effect;
        }

        public void Trigger()
        {
            Console.WriteLine($"Event Triggered: {EventName}");
            Effect.Invoke();
        }
    }
}

// Explanation:
// GameEvent Class: Now includes a console log when an event is triggered for better tracking during development.