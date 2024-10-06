public class Technology : Node
{
    public string Name { get; set; }
    public List<Technology> Prerequisites { get; set; }
    public List<Effect> PositiveEffects { get; set; }
    public List<Effect> NegativeEffects { get; set; }
    public bool IsUnlocked { get; private set; }
    public float BasePublicBenefit { get; set; }
    public float BaseCorporateBenefit { get; set; }
    public float PublicBenefit { get; set; }
    public float CorporateBenefit { get; set; }

    public void Unlock(ResearchFundingSystem fundingSystem)
    {
        IsUnlocked = true;
        fundingSystem.DistributeFunding(this);
        ApplyEffects();
    }

    private void ApplyEffects()
    {
        foreach (var effect in PositiveEffects)
        {
            effect.Apply();
        }
        foreach (var effect in NegativeEffects)
        {
            effect.Apply();
        }

        // Apply public and corporate benefits
        GameState.Instance.PublicWelfare += PublicBenefit;
        GameState.Instance.CorporateProfits += CorporateBenefit;
    }

    public void DisplayInfo(AccessibilityManager accessibilityManager)
    {
        switch (accessibilityManager.CurrentComplexity)
        {
            case AccessibilityManager.ComplexityLevel.Basic:
                DisplayBasicInfo();
                break;
            case AccessibilityManager.ComplexityLevel.Intermediate:
                DisplayIntermediateInfo();
                break;
            case AccessibilityManager.ComplexityLevel.Advanced:
                DisplayAdvancedInfo();
                break;
        }
    }

    private void DisplayBasicInfo()
    {
        // Show only name and a simple description
    }

    private void DisplayIntermediateInfo()
    {
        // Show name, description, and main effects
    }

    private void DisplayAdvancedInfo()
    {
        // Show all details including complex interactions
    }
}

public class Effect
{
    public string Description { get; set; }
    public Action ApplyEffect { get; set; }

    public void Apply()
    {
        ApplyEffect?.Invoke();
    }
}