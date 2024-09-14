using System;
using System.Collections.Generic;
using System.Linq;
using Godot;
using Models;

namespace Systems
{
    public class EventCondition
    {
        public string TimePhase { get; set; }
        public string Weather { get; set; }
        public string PopulationDensity { get; set; }
        public Dictionary<string, string> CustomConditions { get; set; }

        public bool IsMet(GameManager gameManager)
        {
            return CheckTimePhase(gameManager)
                && CheckWeather(gameManager)
                && CheckPopulationDensity(gameManager)
                && CheckCustomConditions(gameManager);
        }

        private bool CheckTimePhase(GameManager gameManager)
        {
            if (string.IsNullOrEmpty(TimePhase))
                return true;

            return gameManager.GetTimeSystem().CurrentDayPhase.ToString() == TimePhase;
        }

        private bool CheckWeather(GameManager gameManager)
        {
            if (string.IsNullOrEmpty(Weather))
                return true;

            return gameManager.GetEnvironmentManager().CurrentWeather.ToString() == Weather;
        }

        private bool CheckPopulationDensity(GameManager gameManager)
        {
            if (string.IsNullOrEmpty(PopulationDensity))
                return true;

            float threshold = ParsePopulationDensity(PopulationDensity);
            return gameManager.Nodes.Any(node => node.Stats.PopulationDensity >= threshold);
        }

        private bool CheckCustomConditions(GameManager gameManager)
        {
            if (CustomConditions == null || CustomConditions.Count == 0)
                return true;

            return CustomConditions.All(condition => CheckCustomCondition(condition.Key, condition.Value, gameManager));
        }

        private bool CheckCustomCondition(string key, string value, GameManager gameManager)
        {
            var parts = value.Split(' ', StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length != 2 || !float.TryParse(parts[1], out float threshold))
            {
                GD.PrintErr($"Invalid custom condition format: {key} {value}");
                return false;
            }

            float statValue = GetStatValue(key, gameManager);
            return parts[0].ToLower() switch
            {
                "min" => statValue >= threshold,
                "max" => statValue <= threshold,
                "equals" => Math.Abs(statValue - threshold) < float.Epsilon,
                _ => false
            };
        }

        private float GetStatValue(string statName, GameManager gameManager)
        {
            return statName.ToLower() switch
            {
                "economicprosperity" => gameManager.GetEconomySystem().EconomicProsperity,
                "morale" => gameManager.Nodes.Average(node => node.Stats.Morale),
                "resourceavailability" => gameManager.Nodes.Average(node => node.Stats.ResourceAvailability),
                "culturaldevelopment" => gameManager.Nodes.Average(node => node.Stats.CulturalDevelopment),
                "populationdensity" => gameManager.Nodes.Average(node => node.Stats.PopulationDensity),
                _ => throw new ArgumentException($"Unknown stat name: {statName}")
            };
        }

        private float ParsePopulationDensity(string density)
        {
            return density.ToLower() switch
            {
                "high" => 70f,
                "medium" => 50f,
                "low" => 30f,
                _ => throw new ArgumentException($"Invalid population density value: {density}")
            };
        }
    }
}

// o1 mini
// Explanation
// CustomConditions: Allows defining additional, flexible conditions for events.
// CheckCustomCondition: Parses and evaluates custom condition logic (e.g., minimum economic prosperity).

// claude sonnet
// This improved version of the EventCondition class incorporates the following enhancements:

// Separated the condition checks into individual methods for better organization and readability.
// Used LINQ for more concise and efficient checks, particularly for population density and custom conditions.
// Improved error handling and logging using Godot's GD.PrintErr method.
// Enhanced the custom condition parsing to support "equals" in addition to "min" and "max".
// Expanded the GetStatValue method to handle more stats, including averages across all nodes where applicable.
// Improved the ParsePopulationDensity method to throw an exception for invalid inputs, helping catch configuration errors early.
// Used pattern matching (switch expressions) for cleaner code in several methods.

// Key improvements and reconciliations:

// Maintained the overall structure and functionality from both versions.
// Improved the efficiency of population density checking by using LINQ's Any method instead of a foreach loop.
// Enhanced custom condition checking to be more flexible and robust.
// Added more comprehensive stat value retrieval, allowing for easy expansion in the future.
// Improved error handling and logging throughout the class.

// To use this improved EventCondition class:

// Ensure that your GameManager, TimeSystem, EnvironmentManager, and Node classes have the necessary properties and methods (e.g., CurrentDayPhase, CurrentWeather, Stats).
// When creating event definitions, you can now use more sophisticated conditions, including custom conditions with "min", "max", and "equals" comparisons.
// The class now provides better error messages, which will help in debugging and setting up event conditions.

// This improved EventCondition class provides a more robust and flexible way to define and check conditions for your game events, while maintaining good performance and code readability.