using Godot;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;

namespace Systems
{
    public static class DataLoader
    {
        public static List<ElectionDefinition> LoadElectionDefinitions(string path)
        {
            string fullPath = ProjectSettings.GlobalizePath(path);
            if (!File.Exists(fullPath))
            {
                GD.PrintErr($"Election definitions file not found: {path}");
                return new List<ElectionDefinition>();
            }

            string json = File.ReadAllText(fullPath);
            var data = JsonConvert.DeserializeObject<ElectionDefinitionsData>(json);
            return data.Elections;
        }

        public static List<CandidateDefinition> LoadCandidateDefinitions(string path)
        {
            string fullPath = ProjectSettings.GlobalizePath(path);
            if (!File.Exists(fullPath))
            {
                GD.PrintErr($"Candidate definitions file not found: {path}");
                return new List<CandidateDefinition>();
            }

            string json = File.ReadAllText(fullPath);
            var data = JsonConvert.DeserializeObject<CandidateDefinitionsData>(json);
            return data.Candidates;
        }
    }

    // Helper classes for JSON deserialization
    public class ElectionDefinitionsData
    {
        [JsonProperty("elections")]
        public List<ElectionDefinition> Elections { get; set; }
    }

    public class ElectionDefinition
    {
        [JsonProperty("title")]
        public string Title { get; set; }

        [JsonProperty("type")]
        public string Type { get; set; }

        [JsonProperty("frequency_in_years")]
        public int FrequencyInYears { get; set; }

        [JsonProperty("applicable_regions")]
        public List<string> ApplicableRegions { get; set; }
    }

    public class CandidateDefinitionsData
    {
        [JsonProperty("candidates")]
        public List<CandidateDefinition> Candidates { get; set; }
    }

    public class CandidateDefinition
    {
        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("election_type")]
        public string ElectionType { get; set; }

        [JsonProperty("platform")]
        public Dictionary<string, float> Platform { get; set; }
    }
}