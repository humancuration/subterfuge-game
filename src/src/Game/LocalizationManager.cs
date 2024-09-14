// File: Localization/LocalizationManager.cs
// Localization System
// Support multiple languages for your game's text content, making it accessible to a wider audience.

using System.Collections.Generic;
using Godot;

namespace Localization
{
    public class LocalizationManager
    {
        private Dictionary<string, Dictionary<string, string>> localizedText;
        private string currentLanguage;

        public LocalizationManager()
        {
            localizedText = new Dictionary<string, Dictionary<string, string>>();
            currentLanguage = "en";
        }

        public void LoadLocalizedText(string language, string filePath)
        {
            var file = new File();
            if (file.FileExists(filePath))
            {
                file.Open(filePath, File.ModeFlags.Read);
                var text = file.GetAsText();
                var parsedText = JSON.Parse(text).Result as Dictionary;
                localizedText[language] = new Dictionary<string, string>();
                foreach (var entry in parsedText)
                {
                    localizedText[language][entry.Key] = entry.Value.ToString();
                }
                file.Close();
            }
        }

        public void SetLanguage(string language)
        {
            if (localizedText.ContainsKey(language))
            {
                currentLanguage = language;
            }
        }

        public string GetText(string key)
        {
            if (localizedText[currentLanguage].ContainsKey(key))
            {
                return localizedText[currentLanguage][key];
            }
            return key;
        }
    }
}