private ComplexGameEvent SelectRandomEligibleEvent()
{
    List<ComplexGameEvent> eligibleEvents = new List<ComplexGameEvent>();

    foreach (var gameEvent in allEvents)
    {
        if (gameEvent.Conditions.IsMet(gameManager))
        {
            eligibleEvents.Add(gameEvent);
        }
    }

    if (eligibleEvents.Count > 0)
    {
        // Weight events based on certain criteria, such as rarity or importance
        // For simplicity, select randomly here
        return eligibleEvents[rnd.Next(eligibleEvents.Count)];
    }

    return null;
}

// o1 mini
// Dynamic Event Selection
// Enhance SelectRandomEligibleEvent to consider the combination of multiple factors dynamically.
// Explanation
// Weighting: You can enhance this method to assign weights to events, making some events more likely to occur than others based on game state or randomness.