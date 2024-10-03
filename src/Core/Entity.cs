// 1. Base Classes and Interfaces
// Entity Base Class

public class Entity
{
    public int Id { get; private set; }
    public string Name { get; set; }

    public Entity(int id)
    {
        Id = id;
    }
}