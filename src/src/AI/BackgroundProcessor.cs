// Running Updates in Separate Threads
// src/BackgroundProcessor.cs

public class BackgroundProcessor
{
    private CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

    public void Start(GameManager gameManager)
    {
        Task.Run(async () =>
        {
            while (!cancellationTokenSource.IsCancellationRequested)
            {
                gameManager.Update(Time.deltaTime);
                await Task.Delay(16); // Approximate 60 FPS
            }
        }, cancellationTokenSource.Token);
    }

    public void Stop()
    {
        cancellationTokenSource.Cancel();
    }
}
