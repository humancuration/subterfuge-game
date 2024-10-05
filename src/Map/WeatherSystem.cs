using System;
using Models;

public class WeatherSystem : IUpdatable
{
    private EnvironmentManager environmentManager;
    private GameManager gameManager;

    public WeatherSystem(EnvironmentManager envManager, GameManager manager)
    {
        environmentManager = envManager;
        gameManager = manager;
    }

    public void Update(float deltaTime)
    {
        environmentManager.UpdateWeather(deltaTime);
    }
}