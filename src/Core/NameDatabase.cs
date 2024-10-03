// This database provides a rich variety of names and concepts that blend historical, contemporary, and futuristic elements, with a focus on solarpunk and cyberpunk themes. Here's a breakdown of the categories:
// 1. NPC Names: A mix of modern, futuristic, and nature-inspired names.
// 2. Node Names: Locations that combine technological and ecological concepts.
// 3. Corporation Names: Companies that focus on sustainable tech and futuristic innovations.
// 4. Group Names: Organizations and movements that reflect various subcultures and ideologies.
// 5. Culture Names: Philosophical and cultural movements that might emerge in a solarpunk future.
// You can easily expand this database by adding more entries to each category or creating new categories as needed. This data can be loaded into your game to randomly generate names for NPCs, locations, and other entities, helping to create a rich and diverse game world.
// To use this in your C# code, you could create a class to load and manage this data:


using System.IO;
using Newtonsoft.Json;

public class NameDatabase
{
    public NameData Data { get; private set; }

    public NameDatabase(string filePath)
    {
        string json = File.ReadAllText(filePath);
        Data = JsonConvert.DeserializeObject<NameData>(json);
    }

    public string GetRandomNPCName()
    {
        string firstName = GetRandomElement(Data.npc_names.first_names);
        string lastName = GetRandomElement(Data.npc_names.last_names);
        return $"{firstName} {lastName}";
    }

    public string GetRandomNodeName()
    {
        return GetRandomElement(Data.node_names);
    }

    // Add similar methods for other categories

    private string GetRandomElement(string[] array)
    {
        return array[new Random().Next(array.Length)];
    }
}

public class NameData
{
    public NPCNames npc_names { get; set; }
    public string[] node_names { get; set; }
    public string[] corporation_names { get; set; }
    public string[] group_names { get; set; }
    public string[] culture_names { get; set; }
}

public class NPCNames
{
    public string[] first_names { get; set; }
    public string[] last_names { get; set; }
}