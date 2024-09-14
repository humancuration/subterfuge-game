using System;
using System.Collections.Generic;
using Models;
using Godot;

namespace Systems
{
    public class Region
    {
        public string Name { get; set; }
        public NodeStats Stats { get; set; }
        public List<Election> Elections { get; set; } = new List<Election>();
        public List<Voter> Voters { get; set; } = new List<Voter>();

        // Constructor
        public Region(string name)
        {
            Name = name;
            Stats = new NodeStats();
            // Initialize voters based on population density, etc.
            InitializeVoters();
        }

        private void InitializeVoters()
        {
            // Example: Generate voters
            for (int i = 0; i < 1000; i++)
            {
                Voters.Add(new Voter());
            }
        }

        public void InitializeElections()
        {
            // Load or create elections based on configuration
            // Example: Add a yearly mayoral election
            Elections.Add(new Election
            {
                Title = "Mayoral Election",
                Type = ElectionType.Mayor,
                FrequencyInYears = 1,
                NextElectionTime = GD.Randf() * 3600f // Randomize initial election time
            });
        }

        public void HandleElections()
        {
            foreach (var election in Elections)
            {
                if (election.NextElectionTime <= 0f)
                {
                    ConductElection(election);
                    election.NextElectionTime = election.FrequencyInYears * 3600f; // Set next election time
                }
                else
                {
                    election.NextElectionTime -= 3600f; // Advance time
                }
            }
        }

        private void ConductElection(Election election)
        {
            // Implement election logic
            election.Candidates = LoadCandidates(election.Type);
            var results = election.Candidates.Select(c => new
            {
                Candidate = c,
                Votes = Voters.Count(v => v.Supports(c, this))
            }).OrderByDescending(r => r.Votes).ToList();

            var winner = results.First().Candidate;
            GD.Print($"Election Winner in {Name}: {winner.Name}");

            // Apply winner's policies or effects
            winner.ApplyPolicies(this);
        }

        private List<Candidate> LoadCandidates(ElectionType type)
        {
            // Load candidates from JSON or generate dynamically
            // Placeholder implementation
            return new List<Candidate>
            {
                new Candidate { Name = "Candidate A", Platform = new Dictionary<string, float> { { "EconomicProsperity", 70f }, { "EnvironmentalHealth", 50f } } },
                new Candidate { Name = "Candidate B", Platform = new Dictionary<string, float> { { "EconomicProsperity", 40f }, { "EnvironmentalHealth", 80f } } }
            };
        }
    }
}