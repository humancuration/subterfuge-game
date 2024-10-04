using NLog;

public static class Logger
{
    private static readonly NLog.Logger logger = NLog.LogManager.GetCurrentClassLogger();

    public static void SetupLogger(string name, string logFilePath)
    {
        var config = new NLog.Config.LoggingConfiguration();

        // Targets where to log to: File and Console
        var logfile = new NLog.Targets.FileTarget("logfile")
        {
            FileName = logFilePath,
            Layout = "${longdate} ${level} ${message} ${exception}"
        };
        var logconsole = new NLog.Targets.ConsoleTarget("logconsole");

        // Rules for mapping loggers to targets            
        config.AddRule(LogLevel.Info, LogLevel.Fatal, logconsole);
        config.AddRule(LogLevel.Info, LogLevel.Fatal, logfile);

        // Apply config           
        NLog.LogManager.Configuration = config;
    }

    public static void LogInfo(string message)
    {
        logger.Info(message);
    }

    public static void LogError(string message)
    {
        logger.Error(message);
    }
}