// File: src/Buildings/Building.cs (extended)
public abstract class Building : Node
{
    public string BuildingName { get; set; }
    public int Cost { get; set; }
    public int UpgradeLevel { get; private set; } = 0;

    public abstract void Activate(Waypoint ownerWaypoint);

    public void UpgradeBuilding()
    {
        UpgradeLevel++;
        GD.Print($"{BuildingName} upgraded to level {UpgradeLevel}");
        // Enhance building effects based on upgrade level
    }
}