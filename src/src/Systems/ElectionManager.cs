using Godot;
using System;
using System.Collections.Generic;

namespace Systems
{
    public class ElectionManager : Node, IUpdatable
    {
        private List<Country> countries;
        private float electionTimer = 0f;

        // Constructor
        public ElectionManager(List<Country> countries)
        {
            this.countries = countries;
        }

        public override void _Ready()
        {
            // Initialize elections based on loaded data
            foreach (var country in countries)
            {
                country.InitializeElections();
            }
            GD.Print("ElectionManager initialized.");
        }

        public void Update(float deltaTime)
        {
            electionTimer -= deltaTime;
            if (electionTimer <= 0f)
            {
                TriggerElections();
                SetNextElectionCycle();
            }
        }

        private void TriggerElections()
        {
            foreach (var country in countries)
            {
                country.HandleElections();
            }
        }

        private void SetNextElectionCycle()
        {
            // Set the timer for the next election cycle (e.g., yearly)
            electionTimer = 3600f; // 1 hour in game time as an example
        }
    }
}