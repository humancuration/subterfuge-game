// File: src/Modding/ModManager.cs
using Godot;
using System;
using System.Collections.Generic;
using System.Reflection;

public class ModManager : Node
{
    private List<Assembly> loadedMods = new List<Assembly>();

    public override void _Ready()
    {
        LoadMods();
    }

    private void LoadMods()
    {
        string modsPath = "res://mods/";
        Directory modsDir = new Directory();
        if (modsDir.DirExists(modsPath))
        {
            modsDir.Open(modsPath);
            modsDir.ListDirBegin();
            string modFile = modsDir.GetNext();
            while (modFile != "")
            {
                if (modFile.EndsWith(".dll"))
                {
                    Assembly modAssembly = Assembly.LoadFile(System.IO.Path.Combine(ProjectSettings.GlobalizePath(modsPath), modFile));
                    loadedMods.Add(modAssembly);
                    GD.Print($"Loaded mod: {modFile}");
                }
                modFile = modsDir.GetNext();
            }
        }
        else
        {
            GD.Print("No mods directory found.");
        }
    }

    // Method to initialize mods, call appropriate methods or hooks
    public void InitializeMods()
    {
        foreach (var mod in loadedMods)
        {
            // Example: Look for a specific interface to initialize
            foreach (Type type in mod.GetTypes())
            {
                if (typeof(IMod).IsAssignableFrom(type) && !type.IsInterface && !type.IsAbstract)
                {
                    IMod modInstance = (IMod)Activator.CreateInstance(type);
                    modInstance.Initialize(this);
                }
            }
        }
    }
    // File: src/Modding/ModManager.cs (extended)
public void RegisterEvent(Action newEvent)
{
    events.Add(newEvent);
    GD.Print("A new event has been registered by a mod.");
}
}

// File: src/Modding/IMod.cs
public interface IMod
{
    void Initialize(ModManager manager);
}