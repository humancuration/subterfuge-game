public class DynamicEffectsSystem : Node
{
    public override void _Process(float delta)
    {
        UpdateTrafficEffects();
        UpdatePollutionEffects();
        UpdateEconomyEffects();
        UpdateResearchEffects();
    }

    private void UpdateTrafficEffects()
    {
        float trafficReduction = GameState.Instance.PublicTransportUsage * 0.5f;
        GameState.Instance.TrafficCongestion = Mathf.Max(0, GameState.Instance.TrafficCongestion - trafficReduction);
        GameState.Instance.AirPollution = Mathf.Max(0, GameState.Instance.AirPollution - (trafficReduction * 0.3f));
    }

    private void UpdatePollutionEffects()
    {
        if (GameState.Instance.AirPollution > 50)
        {
            GameState.Instance.PublicHealth -= 0.1f * delta;
            GameState.Instance.TourismRevenue -= 100 * delta;
        }
    }

    private void UpdateEconomyEffects()
    {
        if (GameState.Instance.TrafficCongestion < 30)
        {
            GameState.Instance.EconomicProductivity += 0.05f * delta;
        }
        else if (GameState.Instance.TrafficCongestion > 70)
        {
            GameState.Instance.EconomicProductivity -= 0.1f * delta;
        }
    }

    private void UpdateResearchEffects()
    {
        float publicRatio = GameState.Instance.PublicResearchProgress / 
            (GameState.Instance.PublicResearchProgress + GameState.Instance.CorporateResearchProgress);

        if (publicRatio > 0.6)
        {
            GameState.Instance.PublicWelfare += 0.1f * delta;
            GameState.Instance.WealthInequality -= 0.05f * delta;
        }
        else if (publicRatio < 0.4)
        {
            GameState.Instance.EconomicProductivity += 0.15f * delta;
            GameState.Instance.WealthInequality += 0.1f * delta;
        }
    }
}