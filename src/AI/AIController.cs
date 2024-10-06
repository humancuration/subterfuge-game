// File: src/AI/AIController.cs
using Godot;
using System.Collections.Generic;

public class AIController : Node
{
    private City currentCity;
    private List<City> visitedCities = new List<City>();

    public void SetStartCity(City startCity)
    {
        currentCity = startCity;
        visitedCities.Add(currentCity);
    }

    public void MoveToNextCity()
    {
        if (currentCity.ConnectedCities.Count == 0)
            return;

        // Simple random movement
        Random rnd = new Random();
        City nextCity = currentCity.ConnectedCities[rnd.Next(currentCity.ConnectedCities.Count)];
        currentCity = nextCity;

        if (!visitedCities.Contains(currentCity))
            visitedCities.Add(currentCity);

        GD.Print($"AI moved to {currentCity.CityName}");
    }
}