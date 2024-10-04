using NUnit.Framework;
using System.Collections.Generic;

[TestFixture]
public class DataPipelineTests
{
    [Test]
    public void TestDataPipeline()
    {
        var dataProcessor = new DataProcessor();
        var rawData = new List<Dictionary<string, string>>
        {
            new Dictionary<string, string> { { "species", "Species A" }, { "genetic_marker", "ATCG" } },
            new Dictionary<string, string> { { "species", "Species B" }, { "genetic_marker", "TTCG" } },
            // Add more test data as needed
        };

        var tree = dataProcessor.ProcessData(rawData);
        Assert.IsNotNull(tree);
        Assert.IsTrue(tree.Count > 0);
    }
}