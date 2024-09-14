// 8. Lazy Loading and Culling

// Implement culling by checking the player's position and only loading entities within a certain range.

public void UpdateVisibleNodes(Vector2 playerPosition)
{
    float renderDistance = 1000f; // Adjust as needed

    foreach (var node in gameManager.GetNodes())
    {
        float distance = (node.Position - playerPosition).Length();

        if (distance <= renderDistance)
        {
            if (!node.IsVisible)
            {
                DisplayNode(node);
                node.IsVisible = true;
            }
        }
        else
        {
            if (node.IsVisible)
            {
                HideNode(node);
                node.IsVisible = false;
            }
        }
    }
}
