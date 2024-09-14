using System.Collections.Generic;

namespace Systems
{
    public enum ElectionType
    {
        Mayor,
        President,
        // Add more types as needed
    }

    public class Election
    {
        public string Title { get; set; }
        public ElectionType Type { get; set; }
        public int FrequencyInYears { get; set; }
        public float NextElectionTime { get; set; } // In game time units

        public List<Candidate> Candidates { get; set; } = new List<Candidate>();
    }
}