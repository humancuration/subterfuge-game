
namespace Models
{
    public class TerrainFeature
    {
        public string Name { get; set; }
        public float MovementCost { get; set; }
        public float ResourceModifier { get; set; }

        public TerrainFeature(string name, float movementCost, float resourceModifier)
        {
            Name = name;
            MovementCost = movementCost;
            ResourceModifier = resourceModifier;
        }

        // Method to serialize terrain feature data
        public Dictionary<string, object> SerializeTerrainFeature()
        {
            return new Dictionary<string, object>
            {
                { "Name", Name },
                { "MovementCost", MovementCost },
                { "ResourceModifier", ResourceModifier }
            };
        }

        // Method to deserialize terrain feature data
        public static TerrainFeature DeserializeTerrainFeature(Dictionary<string, object> data)
        {
            return new TerrainFeature(
                data["Name"].ToString(),
                Convert.ToSingle(data["MovementCost"]),
                Convert.ToSingle(data["ResourceModifier"])
            );
        }
    }

    // Added serialization and deserialization methods for TerrainFeature.
}