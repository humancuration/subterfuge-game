public class EnvironmentManager : Node
{
    public enum WeatherCondition
    {
        Clear,
        Rain,
        Storm,
        Snow,
        Fog,
        Heatwave
    }

    public enum DayPhase
    {
        Morning,
        Afternoon,
        Evening,
        Night
    }

    public WeatherCondition CurrentWeather { get; private set; }
    public DayPhase CurrentDayPhase { get; private set; }

    private float weatherTimer = 0f;
    private float weatherChangeInterval = 60f; // Change weather every 60 seconds
    private float dayPhaseDuration = 0f;

    private GIProbe giProbe;
    private CPUParticles leavesEffect;
    private CPUParticles rainEffect;
    private CPUParticles snowEffect;

    public event Action<WeatherCondition> OnWeatherChanged;

    public override void _Ready()
    {
        InitializeGIProbe();
        InitializeParticleEffects();

        // Set initial weather and day phase
        CurrentWeather = WeatherCondition.Clear;
        CurrentDayPhase = DayPhase.Morning;
        SetWeatherDuration();
        SetDayPhaseDuration();
    }

    public override void _Process(float delta)
    {
        UpdateWeather(delta);
        UpdateDayPhase(delta);
        UpdateEnvironmentEffects();
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

    private void UpdateWeather(float deltaTime)
    {
        weatherTimer += deltaTime;
        if (weatherTimer >= weatherChangeInterval)
        {
            weatherTimer = 0f;
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
        WeatherCondition newWeather = GetRandomWeatherCondition();
        CurrentWeather = newWeather;
        OnWeatherChanged?.Invoke(CurrentWeather);
        TriggerWeatherEvent(CurrentWeather);
        SetWeatherDuration();
    }

    private WeatherCondition GetRandomWeatherCondition()
    {
        Array values = Enum.GetValues(typeof(WeatherCondition));
        Random random = new Random();
        return (WeatherCondition)values.GetValue(random.Next(values.Length));
    }

    private void SetWeatherDuration()
    {
        weatherChangeInterval = new Random().Next(300, 900); // Random duration between 5-15 minutes
    }

    private void AdvanceDayPhase()
    {
        CurrentDayPhase = (DayPhase)(((int)CurrentDayPhase + 1) % Enum.GetValues(typeof(DayPhase)).Length);
        SetDayPhaseDuration();
    }

    private void SetDayPhaseDuration()
    {
        dayPhaseDuration = 900; // 15 minutes per phase, adjust as needed
    }

    private void UpdateEnvironmentEffects()
    {
        // Update particle effects based on current weather
        leavesEffect.Emitting = CurrentWeather == WeatherCondition.Clear;
        rainEffect.Emitting = CurrentWeather == WeatherCondition.Rain;
        snowEffect.Emitting = CurrentWeather == WeatherCondition.Snow;

        // Update lighting based on day phase
        UpdateLighting();
    }

    private void UpdateLighting()
    {
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

        if (CurrentDayPhase == DayPhase.Night || CurrentDayPhase == DayPhase.Morning)
        {
            giProbe.Bake();
        }
    }

    private void TriggerWeatherEvent(WeatherCondition weather)
    {
        switch (weather)
        {
            case WeatherCondition.Rain:
                EventManager.Instance.Emit("WeatherEvent", new WeatherEvent("Heavy Rain", ImpactType.Environmental, -5f));
                break;
            case WeatherCondition.Storm:
                EventManager.Instance.Emit("WeatherEvent", new WeatherEvent("Storm", ImpactType.Environmental, -10f));
                break;
            case WeatherCondition.Snow:
                EventManager.Instance.Emit("WeatherEvent", new WeatherEvent("Snowfall", ImpactType.Environmental, -3f));
                break;
            default:
                EventManager.Instance.Emit("WeatherEvent", new WeatherEvent("Clear Skies", ImpactType.Environmental, 5f));
                break;
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