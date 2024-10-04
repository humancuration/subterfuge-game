using System;
using System.IO;
using Newtonsoft.Json;

public class ConfigSettings
{
    // Visualization Parameters
    public string BackgroundColor { get; set; }
    public string NodeColor { get; set; }
    public string EdgeColor { get; set; }
    public int NodeSize { get; set; }
    public int EdgeWidth { get; set; }

    // File Paths
    public string DataFile { get; set; }
    public string LogFile { get; set; }

    // UI Constants
    public int WindowWidth { get; set; }
    public int WindowHeight { get; set; }

    public static ConfigSettings LoadSettings(string filePath)
    {
        if (File.Exists(filePath))
        {
            string json = File.ReadAllText(filePath);
            return JsonConvert.DeserializeObject<ConfigSettings>(json);
        }
        else
        {
            var defaultSettings = new ConfigSettings
            {
                BackgroundColor = ConfigConstants.BackgroundColor,
                NodeColor = ConfigConstants.NodeColor,
                EdgeColor = ConfigConstants.EdgeColor,
                NodeSize = ConfigConstants.NodeSize,
                EdgeWidth = ConfigConstants.EdgeWidth,
                DataFile = ConfigConstants.DefaultDataFile,
                LogFile = ConfigConstants.LogFile,
                WindowWidth = ConfigConstants.WindowWidth,
                WindowHeight = ConfigConstants.WindowHeight
            };
            SaveSettings(defaultSettings, filePath);
            return defaultSettings;
        }
    }

    public static void SaveSettings(ConfigSettings settings, string filePath)
    {
        string json = JsonConvert.SerializeObject(settings, Formatting.Indented);
        File.WriteAllText(filePath, json);
    }
}

public class VisualizationParams
{
    public string BackgroundColor { get; set; }
    public string NodeColor { get; set; }
    public string EdgeColor { get; set; }
    public int NodeSize { get; set; }
    public int EdgeWidth { get; set; }
}