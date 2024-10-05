using System;
using System.Collections.Generic;
using NLog;

public static class EventManager
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

public abstract class GameEvent
{
    public string EventName { get; private set; }
    public ImpactType ImpactType { get; private set; }
    public float Magnitude { get; private set; }

    protected GameEvent(string name, ImpactType impactType, float magnitude)
    {
        EventName = name;
        ImpactType = impactType;
        Magnitude = magnitude;
    }

    public abstract void ApplyEffect(GameManager gameManager);
}

public class WeatherEvent : GameEvent
{
    public WeatherEvent(string name, ImpactType impactType, float magnitude)
        : base(name, impactType, magnitude)
    {
    }

    public override void ApplyEffect(GameManager gameManager)
    {
        foreach (var node in gameManager.Nodes)
        {
            node.Stats.EnvironmentalHealth += Magnitude;
        }

        foreach (var npc in gameManager.NPCs)
        {
            if (gameManager.EnvironmentManager.CurrentWeather == WeatherCondition.Storm || 
                gameManager.EnvironmentManager.CurrentWeather == WeatherCondition.Snow)
            {
                npc.ChangeState(new DefensiveState());
            }
            else
            {
                npc.ChangeState(new IdleState());
            }
        }

        GD.Print($"Weather Event Triggered: {EventName} with impact {Magnitude}");
    }
}

public enum ImpactType
{
    Environmental,
    Economic,
    Social,
    Health
}