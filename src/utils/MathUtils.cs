using System;
using System.Collections.Generic;
using System.Linq;

public static class MathUtils
{
    public static int HammingDistance(string s1, string s2)
    {
        if (s1.Length != s2.Length)
            return -1; // Undefined for unequal lengths

        return s1.Zip(s2, (c1, c2) => c1 != c2).Count(difference => difference);
    }

    public static double CalculateGeneticDistance(List<string> markers1, List<string> markers2)
    {
        if (markers1 == null || markers2 == null || markers1.Count == 0 || markers2.Count == 0)
            return double.PositiveInfinity;

        List<int> distances = new List<int>();

        foreach (var m1 in markers1)
        {
            foreach (var m2 in markers2)
            {
                int distance = HammingDistance(m1, m2);
                if (distance != -1)
                    distances.Add(distance);
            }
        }

        return distances.Count > 0 ? distances.Average() : double.PositiveInfinity;
    }
}