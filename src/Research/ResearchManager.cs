public class ResearchManager : Node
{
    private AccessibilityManager accessibilityManager;

    public override void _Ready()
    {
        accessibilityManager = GetNode<AccessibilityManager>("/root/AccessibilityManager");
    }

    public void ProcessResearch()
    {
        if (accessibilityManager.AutoManageResearch)
        {
            AutoAllocateResearchPoints();
        }
        else
        {
            // Wait for player input on research allocation
        }
    }

    private void AutoAllocateResearchPoints()
    {
        // Logic for automatically allocating research points
    }
}