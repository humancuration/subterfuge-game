using Godot;
using System;
using System.Collections.Generic;

namespace Models
{
    public class Resource
    {
        public string Name { get; private set; }
        public int Amount { get; set; }

        public Resource(string name, int amount)
        {
            Name = name;
            Amount = amount;
        }
    }

    public class Item
    {
        public string Name { get; private set; }
        public string Description { get; private set; }
        public Dictionary<string, int> CraftingRecipe { get; private set; } = new Dictionary<string, int>();

        public Item(string name, string description)
        {
            Name = name;
            Description = description;
        }

        public void AddResourceToRecipe(string resourceName, int amount)
        {
            CraftingRecipe[resourceName] = amount;
        }
    }
}