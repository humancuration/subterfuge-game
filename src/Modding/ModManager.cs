using Godot;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.IO;
using Newtonsoft.Json;

namespace Modding
{
    public class ModManager : Node
    {
        private List<Assembly> loadedMods = new List<Assembly>();
        private Dictionary<string, IMod> activeMods = new Dictionary<string, IMod>();
        private Dictionary<string, ModMetadata> modMetadata = new Dictionary<string, ModMetadata>();

        [Signal]
        public delegate void ModLoaded(string modName);

        [Signal]
        public delegate void ModEnabled(string modName);

        [Signal]
        public delegate void ModDisabled(string modName);

        public override void _Ready()
        {
            LoadMods();
            InitializeMods();
        }

        private void LoadMods()
        {
            string modsPath = OS.GetExecutablePath().GetBaseDir().PlusFile("mods");
            if (Directory.Exists(modsPath))
            {
                foreach (string modFolder in Directory.GetDirectories(modsPath))
                {
                    string modInfoPath = Path.Combine(modFolder, "mod_info.json");
                    if (File.Exists(modInfoPath))
                    {
                        ModMetadata metadata = JsonConvert.DeserializeObject<ModMetadata>(File.ReadAllText(modInfoPath));
                        modMetadata[metadata.Name] = metadata;

                        string dllPath = Path.Combine(modFolder, metadata.MainAssembly);
                        if (File.Exists(dllPath))
                        {
                            Assembly modAssembly = Assembly.LoadFile(dllPath);
                            loadedMods.Add(modAssembly);
                            GD.Print($"Loaded mod: {metadata.Name} v{metadata.Version}");
                            EmitSignal(nameof(ModLoaded), metadata.Name);
                        }
                        else
                        {
                            GD.PrintErr($"Main assembly not found for mod: {metadata.Name}");
                        }
                    }
                    else
                    {
                        GD.PrintErr($"mod_info.json not found in mod folder: {modFolder}");
                    }
                }
            }
            else
            {
                GD.Print("No mods directory found.");
            }
        }

        public void InitializeMods()
        {
            foreach (var mod in loadedMods)
            {
                foreach (Type type in mod.GetTypes())
                {
                    if (typeof(IMod).IsAssignableFrom(type) && !type.IsInterface && !type.IsAbstract)
                    {
                        IMod modInstance = (IMod)Activator.CreateInstance(type);
                        string modName = modMetadata.FirstOrDefault(x => x.Value.MainAssembly == mod.Location).Key;
                        if (!string.IsNullOrEmpty(modName))
                        {
                            activeMods[modName] = modInstance;
                            modInstance.Initialize(this);
                            GD.Print($"Initialized mod: {modName}");
                        }
                        else
                        {
                            GD.PrintErr($"Failed to find metadata for mod: {type.FullName}");
                        }
                    }
                }
            }
        }

        public void EnableMod(string modName)
        {
            if (activeMods.TryGetValue(modName, out IMod mod))
            {
                mod.Enable();
                EmitSignal(nameof(ModEnabled), modName);
                GD.Print($"Enabled mod: {modName}");
            }
            else
            {
                GD.PrintErr($"Mod not found: {modName}");
            }
        }

        public void DisableMod(string modName)
        {
            if (activeMods.TryGetValue(modName, out IMod mod))
            {
                mod.Disable();
                EmitSignal(nameof(ModDisabled), modName);
                GD.Print($"Disabled mod: {modName}");
            }
            else
            {
                GD.PrintErr($"Mod not found: {modName}");
            }
        }

        public void RegisterCustomContent(string contentType, object content)
        {
            // Implementation depends on your game's content system
            GD.Print($"Registered new {contentType} from a mod.");
        }

        public List<ModMetadata> GetAvailableMods()
        {
            return new List<ModMetadata>(modMetadata.Values);
        }
    }

    public interface IMod
    {
        void Initialize(ModManager manager);
        void Enable();
        void Disable();
    }

    public class ModMetadata
    {
        public string Name { get; set; }
        public string Version { get; set; }
        public string Author { get; set; }
        public string Description { get; set; }
        public string MainAssembly { get; set; }
        public List<string> Dependencies { get; set; }
    }
}

// This improved modding system includes the following enhancements:

// Mod metadata: Each mod now requires a mod_info.json file with metadata about the mod.
// Improved mod loading: Mods are now loaded from individual folders, allowing for better organization and potential resource inclusion.
// Mod enabling/disabling: Added methods to enable and disable mods at runtime.
// Custom content registration: Added a method for mods to register custom content with the game.
// Mod listing: Added a method to get a list of available mods.
// Signals: Added signals for mod loading, enabling, and disabling events.
// Error handling: Improved error messages for various failure scenarios.

// To use this system:

// Create a "mods" folder in your game's executable directory.
// For each mod, create a subfolder with a mod_info.json file and the mod's DLL.
// Implement the IMod interface in your mod's main class.
// Use the ModManager in your game to load, initialize, enable, and disable mods.