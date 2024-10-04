using System;
using System.Collections.Generic;
using NLog;

public class EventManager
{
    private static EventManager instance;
    private NLog.Logger logger = LogManager.GetCurrentClassLogger();

    public static EventManager Instance => instance ?? (instance = new EventManager());

    private Dictionary<string, List<Action<object>>> listeners = new Dictionary<string, List<Action<object>>>();

    private EventManager() { }

    public void Subscribe(string eventType, Action<object> listener)
    {
        if (!listeners.ContainsKey(eventType))
            listeners[eventType] = new List<Action<object>>();
        listeners[eventType].Add(listener);
    }

    public void Emit(string eventType, object data)
    {
        if (listeners.ContainsKey(eventType))
        {
            foreach (var listener in listeners[eventType])
            {
                try
                {
                    listener.Invoke(data);
                }
                catch (Exception ex)
                {
                    logger.Error($"Error handling event {eventType}: {ex.Message}");
                }
            }
        }
    }

    public void Unsubscribe(string eventType, Action<object> listener)
    {
        if (listeners.ContainsKey(eventType))
        {
            listeners[eventType].Remove(listener);
        }
    }
}