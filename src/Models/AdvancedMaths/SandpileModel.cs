// Complex Systems and Emergence
// Self-Organized Criticality and the Sandpile Model

using System;

namespace Models
{
    public class SandpileModel
    {
        private int[,] grid;
        private int criticalThreshold;
        private int rows;
        private int cols;

        public SandpileModel(int rows, int cols, int criticalThreshold)
        {
            this.rows = rows;
            this.cols = cols;
            this.criticalThreshold = criticalThreshold;
            grid = new int[rows, cols];
        }

        public void AddGrain(int x, int y)
        {
            grid[x, y]++;
            Topple();
        }

        private void Topple()
        {
            bool hasToppled;
            do
            {
                hasToppled = false;
                for (int i = 0; i < rows; i++)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        if (grid[i, j] >= criticalThreshold)
                        {
                            grid[i, j] -= 4;
                            // Topple to neighbors
                            if (i > 0) grid[i - 1, j]++;
                            if (i < rows - 1) grid[i + 1, j]++;
                            if (j > 0) grid[i, j - 1]++;
                            if (j < cols - 1) grid[i, j + 1]++;
                            hasToppled = true;
                        }
                    }
                }
            } while (hasToppled);
        }

        public void Display()
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    Console.Write(grid[i, j] + " ");
                }
                Console.WriteLine();
            }
        }
    }
}

// Explanation:
// This implementation models the Bak-Tang-Wiesenfeld (BTW) Sandpile Model.
// Grains are added to specific sites, and when a site's grain count exceeds the critical threshold, it topples, distributing grains to its neighbors.
// The Topple method ensures the system reaches a stable state.