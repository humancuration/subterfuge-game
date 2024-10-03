using Godot;
using System.Collections.Generic;

namespace Systems
{
    public class VotingInfluencer : Node
    {
        private GameManager gameManager;

        // Constructor
        public VotingInfluencer(GameManager manager)
        {
            gameManager = manager;
        }

        public override void _Ready()
        {
            // Connect to events or UI actions that influence voting
        }

        // Example method to influence a candidate's support
        public void BoostCandidateSupport(string candidateName, float amount, Region region = null)
        {
            foreach (var country in gameManager.Countries)
            {
                foreach (var election in country.NationalElections)
                {
                    if (election.Candidates.Exists(c => c.Name == candidateName))
                    {
                        // Implement logic to boost support, e.g., increase loyalty of voters
                        foreach (var voter in GetVotersForCandidate(election, candidateName, country))
                        {
                            voter.Loyalty = Mathf.Clamp(voter.Loyalty + amount, 0f, 100f);
                        }
                    }
                }

                foreach (var reg in country.Regions)
                {
                    if (region == null || reg.Name == region.Name)
                    {
                        foreach (var election in reg.Elections)
                        {
                            if (election.Candidates.Exists(c => c.Name == candidateName))
                            {
                                foreach (var voter in GetVotersForCandidate(election, candidateName, country))
                                {
                                    voter.Loyalty = Mathf.Clamp(voter.Loyalty + amount, 0f, 100f);
                                }
                            }
                        }
                    }
                }
            }
        }

        private List<Voter> GetVotersForCandidate(Election election, string candidateName, Country country)
        {
            var voters = new List<Voter>();
            foreach (var region in country.Regions)
            {
                foreach (var voter in region.Voters)
                {
                    if (voter.Supports(election.Candidates.Find(c => c.Name == candidateName), region))
                    {
                        voters.Add(voter);
                    }
                }
            }
            return voters;
        }

        // Additional methods to decrease support, influence entire regions, etc.
    }
}