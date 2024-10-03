public class CoolMod : IMod
{
    private ModManager manager;

    public void Initialize(ModManager manager)
    {
        this.manager = manager;
        // Perform any necessary initialization
    }

    public void Enable()
    {
        // Register custom content
        manager.RegisterCustomContent("event", new CoolEvent());
        // Enable mod features
    }

    public void Disable()
    {
        // Disable mod features
    }
}