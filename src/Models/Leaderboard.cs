using Godot;
using System;
using System.Collections.Generic;

namespace Models
{
    public class Leaderboard
    {
        public List<PlayerStats> PlayerStatsList { get; private set; } = new List<PlayerStats>();

        public Leaderboard()
        {
            // Initialize leaderboard with default player stats
            // or load from a file
        }

        public void AddPlayerStats(PlayerStats playerStats)
        {
            PlayerStatsList.Add(playerStats);
        }

        public void UpdateLeaderboards(GameManager gameManager)
        {
            // Implement logic to update leaderboards based on player stats
            // Example: Sort the list of player stats by a specific stat
            // Example: Display the top 10 players on the leaderboard UI
        }
    }

    public class PlayerStats
    {
        public string PlayerName { get; set; }
        public float Morale { get; set; }
        public float ResourceAvailability { get; set; }
        public float PopulationGrowth { get; set; }
        public float TechnologicalAdvancement { get; set; }
        public float SocialHappiness { get; set; }

        public PlayerStats(string playerName)
        {
            PlayerName = playerName;
        }
    }
}