public class ResearchFundingSystem : Node
{
    public float PublicFunding { get; private set; }
    public float CorporateFunding { get; private set; }

    public void AllocatePublicFunding(float amount)
    {
        PublicFunding += amount;
        GameState.Instance.PublicBudget -= amount;
    }

    public void AllocateCorporateFunding(float amount)
    {
        CorporateFunding += amount;
        GameState.Instance.CorporateProfits -= amount;
    }

    public void DistributeFunding(Technology tech)
    {
        float totalFunding = PublicFunding + CorporateFunding;
        float publicInfluence = PublicFunding / totalFunding;
        float corporateInfluence = CorporateFunding / totalFunding;

        tech.PublicBenefit = tech.BasePublicBenefit * publicInfluence;
        tech.CorporateBenefit = tech.BaseCorporateBenefit * corporateInfluence;
    }
}