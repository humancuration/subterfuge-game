// File: src/Events/EventManager.cs
using Godot;
using System;
using System.Collections.Generic;

public class EventManager : Node
{
    private List<Action> events = new List<Action>();

    public override void _Ready()
    {
        // Initialize with some events
        events.Add(EventTreasure);
        events.Add(EventAmbush);
        events.Add(EventFindAlly);
    }

    public void TriggerRandomEvent()
    {
        Random rnd = new Random();
        int index = rnd.Next(events.Count);
        events[index].Invoke();
    }

    private void EventTreasure()
    {
        GD.Print("You found a hidden treasure! +50 Gold.");
        // Implement gold addition logic
    }

    private void EventAmbush()
    {
        GD.Print("An enemy ambushes you!");
        // Implement combat logic
    }

    private void EventFindAlly()
    {
        GD.Print("You meet a wandering ally who joins your quest.");
        // Implement ally addition logic
    }
}