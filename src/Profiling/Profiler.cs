using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;
using NLog;
using System;
using System.Diagnostics;

public class Profiler
{
    private NLog.Logger logger = LogManager.GetCurrentClassLogger();

    public void RunProfiler()
    {
        var summary = BenchmarkRunner.Run<PerformanceTests>();
        logger.Info("Profiling completed.");
    }

    public void PrintProfilerStats()
    {
        // BenchmarkDotNet automatically handles reporting
    }
}

public class PerformanceTests
{
    [Benchmark]
    public void TestProcessData()
    {
        // Replace with actual processing logic to benchmark
        var dataProcessor = new DataProcessor();
        var rawData = new List<Dictionary<string, string>>(); // Populate with test data
        dataProcessor.ProcessData(rawData);
    }

    [Benchmark]
    public void TestVisualizationUpdate()
    {
        // Replace with actual visualization update logic to benchmark
        var visualizationManager = new VisualizationManager();
        visualizationManager.UpdateVisualization();
    }
}