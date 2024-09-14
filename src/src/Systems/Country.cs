using System;
using System.Collections.Generic;

namespace Systems
{
    public class Country
    {
        public string Name { get; set; }
        public List<Region> Regions { get; set; } = new List<Region>();
        public List<Election> NationalElections { get; set; } = new List<Election>();

        // Constructor
        public Country(string name)
        {
            Name = name;
            // Initialize regions
            InitializeRegions();
        }

        private void InitializeRegions()
        {
            // Example: Create regions
            Regions.Add(new Region("Northern Plains"));
            Regions.Add(new Region("Eastern Hills"));
            // Add more regions as needed
        }

        public void InitializeElections()
        {
            // Initialize national elections
            NationalElections.Add(new Election
            {
                Title = "Presidential Election",
                Type = ElectionType.President,
                FrequencyInYears = 4,
                NextElectionTime = 3600f * 4 // 4 years
            });

            // Initialize regional elections
            foreach (var region in Regions)
            {
                region.InitializeElections();
            }
        }

        public void HandleElections()
        {
            foreach (var election in NationalElections)
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

            // Handle regional elections
            foreach (var region in Regions)
            {
                region.HandleElections();
            }
        }

        private void ConductElection(Election election)
        {
            // Implement national election logic
            election.Candidates = LoadCandidates(election.Type);
            var results = election.Candidates.Select(c => new
            {
                Candidate = c,
                Votes = 0 // Sum votes from all regions
            }).ToList();

            foreach (var region in Regions)
            {
                foreach (var candidate in election.Candidates)
                {
                    results.Find(r => r.Candidate == candidate).Votes += region.Voters.Count(v => v.Supports(candidate, region));
                }
            }

            results = results.OrderByDescending(r => r.Votes).ToList();
            var winner = results.First().Candidate;
            GD.Print($"National Election Winner in {Name}: {winner.Name}");

            // Apply winner's policies or effects
            winner.ApplyPolicies(this);
        }

        private List<Candidate> LoadCandidates(ElectionType type)
        {
            // Load candidates from JSON or generate dynamically
            // Placeholder implementation
            return new List<Candidate>
            {
                new Candidate { Name = "President X", Platform = new Dictionary<string, float> { { "EconomicProsperity", 65f }, { "EnvironmentalHealth", 60f } } },
                new Candidate { Name = "President Y", Platform = new Dictionary<string, float> { { "EconomicProsperity", 45f }, { "EnvironmentalHealth", 85f } } }
            };
        }
    }
}