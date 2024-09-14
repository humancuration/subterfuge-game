using System;
using Models;
using Godot;

namespace Systems
{
    public class Voter
    {
        public float Loyalty { get; set; } = 50f; // Influence by player/actions
        public float Independence { get; set; } = 50f; // Tendency to follow stats vs. player influence

        public bool Supports(Candidate candidate, Region region)
        {
            // Calculate support based on candidate platform and region stats
            float supportScore = 0f;
            foreach (var platform in candidate.Platform)
            {
                if (region.Stats.GetStatValue(platform.Key) >= platform.Value)
                {
                    supportScore += 10f; // Higher support if region stat meets/exceeds candidate platform
                }
                else
                {
                    supportScore -= 5f; // Lower support otherwise
                }
            }

            // Incorporate loyalty and independence
            supportScore += (Loyalty / 100f) * 10f;
            supportScore -= (Independence / 100f) * 5f;

            // Determine support based on supportScore threshold
            return supportScore > 0f;
        }
    }
}