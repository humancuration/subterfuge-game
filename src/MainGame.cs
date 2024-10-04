using Godot;
using System;

public class MainGame : Node
{
    private GameManager gameManager;
    private VisualizationManager visualizationManager;
    private UserInterfaceManager uiManager;
    private EventManager eventManager;
    private Profiler profiler;

    public override void _Ready()
    {
        // Initialize Logger
        LoggerSetup.SetupLogger("SubterfugeGame", ConfigConstants.LogFile);
        Logger.LogInfo("Game initialized.");

        // Load settings
        ConfigSettings settings = ConfigSettings.LoadSettings("config.json");

        // Initialize GameManager
        gameManager = new GameManager();
        AddChild(gameManager);

        // Initialize Visualization
        visualizationManager = new VisualizationManager();
        AddChild(visualizationManager);

        // Initialize UI
        uiManager = GetNode<UserInterfaceManager>("UserInterfaceManager");

        // Initialize EventManager
        eventManager = EventManager.Instance;

        // Initialize Profiler
        profiler = new Profiler();

        Logger.LogInfo("Main game components initialized.");
    }

    public override void _Process(float delta)
    {
        // Update GameManager
        gameManager.Update(delta);

        // Update other systems if necessary
    }

    public void RunProfiling()
    {
        profiler.RunProfiler();
    }

    public void DisplayProfilerStats()
    {
        profiler.PrintProfilerStats();
    }
}