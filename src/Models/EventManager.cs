using Godot;
using System;
using System.Collections.Generic;

namespace Models
{
    public class GameEvent
    {
        public string EventName { get; private set; }
        public string EventDescription { get; private set; }
        public Action<Node> EventAction { get; private set; }

        public GameEvent(string eventName, string eventDescription, Action<Node> eventAction)
        {
            EventName = eventName;
            EventDescription = eventDescription;
            EventAction = eventAction;
        }

        public void Trigger(Node node)
        {
            EventAction?.Invoke(node);
        }
    }

    public static class EventManager
    {
        private static List<GameEvent> events = new List<GameEvent>();

        public static void AddEvent(GameEvent gameEvent)
        {
            events.Add(gameEvent);
        }

        public static void TriggerEvent(string eventName, Node node)
        {
            GameEvent gameEvent = events.Find(e => e.EventName == eventName);
            if (gameEvent != null)
            {
                gameEvent.Trigger(node);
            }
        }

        public static void TriggerRandomEvent(Node node)
        {
            if (events.Count > 0)
            {
                Random rnd = new Random();
                int randomIndex = rnd.Next(0, events.Count);
                events[randomIndex].Trigger(node);
            }
        }
    }
}