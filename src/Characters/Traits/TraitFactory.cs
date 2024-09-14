// 6.4. Extending NPC Trait Factory
// Update the TraitFactory to include the new trait.
// File: Systems/SaveLoadManager.cs

// ... Existing using directives ...

namespace Systems
{
    // ... Existing classes ...

    public static class TraitFactory
    {
        public static ITrait CreateTrait(string traitName)
        {
            switch (traitName)
            {
                case "Optimist":
                    return new OptimistTrait();
                case "Saboteur":
                    return new SaboteurTrait();
                case "Tech Savvy":
                    return new TechSavvyTrait();
                // Add other traits as needed
                default:
                    return null;
            }
        }
    }
}

// Explanation:
// TraitFactory Class: Now recognizes the "Tech Savvy" trait and can instantiate it during the loading process.