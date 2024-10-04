using Godot;
using System.Collections.Generic;
using System.Linq;
using NLog;

public class VisualizationManager : Node
{
    private NLog.Logger logger = LogManager.GetCurrentClassLogger();
    private GameManager gameManager;

    // Visualization parameters
    private Color backgroundColor;
    private Color nodeColor;
    private Color edgeColor;
    private float nodeSize;
    private float edgeWidth;

    // Nodes and edges for visualization
    private List<MeshInstance> visualNodes = new List<MeshInstance>();
    private List<Line2D> visualEdges = new List<Line2D>();

    public override void _Ready()
    {
        gameManager = GetNode<GameManager>("/root/GameManager");
        LoadConfig();
        InitializeVisualization();
    }

    private void LoadConfig()
    {
        ConfigSettings settings = ConfigSettings.LoadSettings("config.json");
        backgroundColor = new Color(settings.BackgroundColor);
        nodeColor = new Color(settings.NodeColor);
        edgeColor = new Color(settings.EdgeColor);
        nodeSize = settings.NodeSize;
        edgeWidth = settings.EdgeWidth;
    }

    private void InitializeVisualization()
    {
        // Set background color
        GetViewport().SetClearColor(backgroundColor);
        
        // Iterate through processed data to create visual nodes and edges
        var processedData = gameManager.DataProcessor.ProcessData(gameManager.RawData);
        var species = processedData.Keys.ToList();

        foreach (var sp in species)
        {
            CreateVisualNode(sp, processedData[sp]["position"]);
        }

        foreach (var sp1 in species)
        {
            foreach (var sp2 in species)
            {
                if (sp1 != sp2)
                {
                    CreateVisualEdge(sp1, sp2, processedData[sp1][sp2]);
                }
            }
        }
    }

    private void CreateVisualNode(string species, Vector3 position)
    {
        MeshInstance node = new MeshInstance();
        node.Mesh = new SphereMesh
        {
            Radius = nodeSize
        };
        node.MaterialOverride = new SpatialMaterial
        {
            AlbedoColor = nodeColor
        };
        node.Translation = position;
        AddChild(node);
        visualNodes.Add(node);
    }

    private void CreateVisualEdge(string sp1, string sp2, double distance)
    {
        // Create a Line2D for simplicity; consider using 3D lines for more complexity
        Line2D edge = new Line2D();
        edge.DefaultColor = edgeColor;
        edge.Width = edgeWidth;

        Vector3 pos1 = gameManager.VisualPositions[sp1];
        Vector3 pos2 = gameManager.VisualPositions[sp2];

        edge.AddPoint(new Vector2(pos1.x, pos1.y));
        edge.AddPoint(new Vector2(pos2.x, pos2.y));
        AddChild(edge);
        visualEdges.Add(edge);
    }

    public void UpdateVisualization()
    {
        // Clear existing visuals
        foreach (var node in visualNodes)
        {
            node.QueueFree();
        }
        visualNodes.Clear();

        foreach (var edge in visualEdges)
        {
            edge.QueueFree();
        }
        visualEdges.Clear();

        // Re-initialize visualization
        InitializeVisualization();
    }

    // Optional: Handle dynamic updates based on game events
    // ...
}