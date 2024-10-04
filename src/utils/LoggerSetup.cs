using NLog;

public static class LoggerSetup
{
    public static void SetupLogger(string name, string logFilePath)
    {
        Logger.SetupLogger(name, logFilePath);
    }
}