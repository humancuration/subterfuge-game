using System.Collections.Generic;

namespace Systems
{
    public class Candidate
    {
        public string Name { get; set; }
        public Dictionary<string, float> Platform { get; set; } = new Dictionary<string, float>();

        public void ApplyPolicies(Country country)
        {
            // Apply policies based on platform
            foreach (var policy in Platform)
            {
                switch (policy.Key)
                {
                    case "EconomicProsperity":
                        country.Regions.ForEach(r => r.Stats.EconomicProsperity = Mathf.Clamp(r.Stats.EconomicProsperity + policy.Value, 0f, 100f));
                        break;
                    case "EnvironmentalHealth":
                        country.Regions.ForEach(r => r.Stats.EnvironmentalHealth = Mathf.Clamp(r.Stats.EnvironmentalHealth + policy.Value, 0f, 100f));
                        break;
                    // Add more policies as needed
                }
            }
        }
    }
}