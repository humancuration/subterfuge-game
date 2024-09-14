using Godot;
using System;

public class EnvironmentManager : Node
{
    public enum WeatherType
    {
        Sunny,
        Rainy,
        Snowy,
        // Add more weather types as needed
    }

    public enum DayPhase
    {
        Morning,
        Afternoon,
        Evening,
        Night
    }

    public WeatherType CurrentWeather { get; private set; }
    public DayPhase CurrentDayPhase { get; private set; }

    private float weatherDuration = 0f;
    private float dayPhaseDuration = 0f;

    private GIProbe giProbe;
    private CPUParticles leavesEffect;
    private CPUParticles rainEffect;
    private CPUParticles snowEffect; // Added for snowy weather

    public override void _Ready()
    {
        InitializeGIProbe();
        InitializeParticleEffects();
        
        // Set initial weather and day phase
        CurrentWeather = WeatherType.Sunny;
        CurrentDayPhase = DayPhase.Morning;
        SetWeatherDuration();
        SetDayPhaseDuration();
    }

    private void InitializeGIProbe()
    {
        giProbe = new GIProbe();
        AddChild(giProbe);
        giProbe.BoxExtents = new Vector3(100, 100, 100); // Adjust to your map size
        giProbe.Mode = GIProbe.ModeEnum.Static;
        giProbe.Bake();
    }

    private void InitializeParticleEffects()
    {
        leavesEffect = (CPUParticles)GD.Load<PackedScene>("res://src/Graphics/Effects/LeavesEffect.tscn").Instance();
        AddChild(leavesEffect);
        leavesEffect.Position = Vector3.Zero;
        leavesEffect.Emitting = false;

        rainEffect = (CPUParticles)GD.Load<PackedScene>("res://src/Graphics/Effects/RainEffect.tscn").Instance();
        AddChild(rainEffect);
        rainEffect.Position = Vector3.Zero;
        rainEffect.Emitting = false;

        snowEffect = (CPUParticles)GD.Load<PackedScene>("res://src/Graphics/Effects/SnowEffect.tscn").Instance();
        AddChild(snowEffect);
        snowEffect.Position = Vector3.Zero;
        snowEffect.Emitting = false;
    }

    public void Update(float deltaTime)
    {
        UpdateWeather(deltaTime);
        UpdateDayPhase(deltaTime);
        UpdateEnvironmentEffects();
    }

    private void UpdateWeather(float deltaTime)
    {
        weatherDuration -= deltaTime;
        if (weatherDuration <= 0f)
        {
            ChangeWeather();
        }
    }

    private void UpdateDayPhase(float deltaTime)
    {
        dayPhaseDuration -= deltaTime;
        if (dayPhaseDuration <= 0f)
        {
            AdvanceDayPhase();
        }
    }

    private void ChangeWeather()
    {
        WeatherType newWeather = (WeatherType)new Random().Next(Enum.GetValues(typeof(WeatherType)).Length);
        CurrentWeather = newWeather;
        SetWeatherDuration();
        UpdateEnvironmentEffects();
    }

    private void AdvanceDayPhase()
    {
        CurrentDayPhase = (DayPhase)(((int)CurrentDayPhase + 1) % Enum.GetValues(typeof(DayPhase)).Length);
        SetDayPhaseDuration();
        UpdateEnvironmentEffects();
    }

    private void SetWeatherDuration()
    {
        weatherDuration = new Random().Next(300, 900); // Random duration between 5-15 minutes
    }

    private void SetDayPhaseDuration()
    {
        dayPhaseDuration = 900; // 15 minutes per phase, adjust as needed
    }

    private void UpdateEnvironmentEffects()
    {
        // Update particle effects based on current weather
        leavesEffect.Emitting = CurrentWeather == WeatherType.Sunny;
        rainEffect.Emitting = CurrentWeather == WeatherType.Rainy;
        snowEffect.Emitting = CurrentWeather == WeatherType.Snowy;

        // Update lighting based on day phase
        UpdateLighting();
    }

    private void UpdateLighting()
    {
        // Implement logic to adjust lighting based on CurrentDayPhase
        // This could involve changing the intensity and color of your main light source
        // For example:
        DirectionalLight sunLight = GetNode<DirectionalLight>("SunLight");
        switch (CurrentDayPhase)
        {
            case DayPhase.Morning:
                sunLight.LightColor = new Color(1.0f, 0.9f, 0.7f);
                sunLight.LightEnergy = 0.7f;
                break;
            case DayPhase.Afternoon:
                sunLight.LightColor = new Color(1.0f, 1.0f, 1.0f);
                sunLight.LightEnergy = 1.0f;
                break;
            case DayPhase.Evening:
                sunLight.LightColor = new Color(1.0f, 0.6f, 0.3f);
                sunLight.LightEnergy = 0.5f;
                break;
            case DayPhase.Night:
                sunLight.LightColor = new Color(0.05f, 0.05f, 0.1f);
                sunLight.LightEnergy = 0.1f;
                break;
        }

        // You might want to update GIProbe for major lighting changes
        if (CurrentDayPhase == DayPhase.Night || CurrentDayPhase == DayPhase.Morning)
        {
            giProbe.Bake();
        }
    }

    public void ApplyWeatherEffects(Node node)
    {
        // Implement logic to apply weather effects to a node
        // Example: Modify node stats or population behavior based on the current weather
        switch (CurrentWeather)
        {
            case WeatherType.Rainy:
                // Slow down movement, increase resource generation, etc.
                break;
            case WeatherType.Snowy:
                // Severely slow down movement, decrease resource generation, etc.
                break;
            // Add more cases as needed
        }
    }

    public void ApplyDayPhaseEffects(Node node)
    {
        // Implement logic to apply day phase effects to a node
        // Example: Adjust NPC routines, events, or dialogue options based on the current day phase
        switch (CurrentDayPhase)
        {
            case DayPhase.Night:
                // Reduce NPC activity, increase stealth effectiveness, etc.
                break;
            case DayPhase.Morning:
                // Increase productivity, trigger "start of day" events, etc.
                break;
            // Add more cases as needed
        }
    }
}

/* This combined version of the EnvironmentManager includes:
Weather system with different types (Sunny, Rainy, Snowy).
Day/night cycle with different phases (Morning, Afternoon, Evening, Night).
Global Illumination using GIProbe.
Particle effects for different weather conditions (leaves, rain, snow).
Methods to apply weather and day phase effects to game nodes.
Dynamic lighting changes based on the time of day.
To use this EnvironmentManager:
Add it to your scene tree.
Call the Update(float deltaTime) method in your game loop.
Use ApplyWeatherEffects(Node node) and ApplyDayPhaseEffects(Node node) to affect game entities based on current environmental conditions.
Remember to create the necessary particle effect scenes (LeavesEffect.tscn, RainEffect.tscn, SnowEffect.tscn) and adjust the lighting node ("SunLight") to match your scene structure.
This implementation provides a solid foundation for an immersive and dynamic environment in your game. You can further expand on this by adding more weather types, refining the lighting changes, or implementing more complex environmental interactions.*/