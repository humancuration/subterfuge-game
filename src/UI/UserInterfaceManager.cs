using Godot;
using System.Collections.Generic;
using NLog;

public class UserInterfaceManager : Control
{
    private readonly NLog.Logger logger = LogManager.GetCurrentClassLogger();
    private GameManager gameManager;

    // UI elements
    private Button zoomInButton;
    private Button zoomOutButton;
    private Button resetViewButton;
    private Button loadDataButton;
    private Button showLogsButton;
    private Slider nodeSizeSlider;
    private Slider lodSlider;
    private Label legendLabel;
    private VBoxContainer legendLayout;
    private LogViewer logViewer;

    public override void _Ready()
    {
        gameManager = GetNode<GameManager>("/root/GameManager");
        SetupUI();
    }

    private void SetupUI()
    {
        // Initialize buttons
        zoomInButton = GetNode<Button>("ZoomInButton");
        zoomOutButton = GetNode<Button>("ZoomOutButton");
        resetViewButton = GetNode<Button>("ResetViewButton");
        loadDataButton = GetNode<Button>("LoadDataButton");
        showLogsButton = GetNode<Button>("ShowLogsButton");

        // Initialize sliders
        nodeSizeSlider = GetNode<Slider>("NodeSizeSlider");
        lodSlider = GetNode<Slider>("LodSlider");

        // Initialize legend
        legendLabel = GetNode<Label>("LegendLabel");
        legendLayout = GetNode<VBoxContainer>("LegendLayout");
        PopulateLegend();

        // Connect signals
        zoomInButton.Connect("pressed", this, nameof(OnZoomInPressed));
        zoomOutButton.Connect("pressed", this, nameof(OnZoomOutPressed));
        resetViewButton.Connect("pressed", this, nameof(OnResetViewPressed));
        loadDataButton.Connect("pressed", this, nameof(OnLoadDataPressed));
        showLogsButton.Connect("pressed", this, nameof(OnShowLogsPressed));

        nodeSizeSlider.Connect("value_changed", this, nameof(OnNodeSizeChanged));
        lodSlider.Connect("value_changed", this, nameof(OnLodChanged));

        // Initialize LogViewer
        logViewer = new LogViewer();
        AddChild(logViewer);
        logViewer.Hide();
    }

    private void PopulateLegend()
    {
        var legendItems = GetLegendItems();
        foreach (var item in legendItems)
        {
            HBoxContainer hBox = new HBoxContainer();
            ColorRect colorRect = new ColorRect
            {
                Color = new Color(item.Color),
                RectSize = new Vector2(20, 20)
            };
            Label textLabel = new Label
            {
                Text = item.Description
            };
            hBox.AddChild(colorRect);
            hBox.AddChild(textLabel);
            legendLayout.AddChild(hBox);
        }
    }

    private List<(string Color, string Description)> GetLegendItems()
    {
        return new List<(string, string)>
        {
            ("#FF5733", "Species A"),
            ("#C70039", "Species B"),
            // Add more legend items as needed
        };
    }

    private void OnZoomInPressed()
    {
        gameManager.VisualizationManager.ZoomIn();
    }

    private void OnZoomOutPressed()
    {
        gameManager.VisualizationManager.ZoomOut();
    }

    private void OnResetViewPressed()
    {
        gameManager.VisualizationManager.ResetView();
    }

    private void OnLoadDataPressed()
    {
        gameManager.LoadData();
        gameManager.VisualizationManager.UpdateVisualization();
    }

    private void OnShowLogsPressed()
    {
        logViewer.Show();
    }

    private void OnNodeSizeChanged(float value)
    {
        gameManager.VisualizationManager.SetNodeSize(value);
        gameManager.VisualizationManager.UpdateVisualization();
    }

    private void OnLodChanged(float value)
    {
        gameManager.VisualizationManager.SetLodThreshold(value);
        gameManager.VisualizationManager.UpdateVisualization();
    }
}