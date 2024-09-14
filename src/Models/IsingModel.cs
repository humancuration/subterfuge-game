// Statistical Mechanics
// Ising Model
// File: Models/IsingModel.cs
using System;

namespace Models
{
    public class IsingModel
    {
        private int[,] spins;
        private int size;
        private double J; // Interaction energy
        private double H; // External magnetic field
        private Random rand;

        public IsingModel(int size, double interactionEnergy, double magneticField)
        {
            this.size = size;
            J = interactionEnergy;
            H = magneticField;
            spins = new int[size, size];
            rand = new Random();
            InitializeSpins();
        }

        private void InitializeSpins()
        {
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    spins[i, j] = rand.Next(0, 2) == 0 ? -1 : 1;
        }

        public void MetropolisStep()
        {
            int i = rand.Next(size);
            int j = rand.Next(size);
            int deltaE = 2 * spins[i, j] * (J * GetNeighborsSum(i, j) + (int)H);
            if (deltaE < 0 || rand.NextDouble() < Math.Exp(-deltaE / 1.0)) // Assume temperature T=1
            {
                spins[i, j] *= -1;
            }
        }

        private int GetNeighborsSum(int i, int j)
        {
            int sum = 0;
            sum += spins[(i + size - 1) % size, j];
            sum += spins[(i + 1) % size, j];
            sum += spins[i, (j + size - 1) % size];
            sum += spins[i, (j + 1) % size];
            return sum;
        }

        public void Simulate(int steps)
        {
            for (int step = 0; step < steps; step++)
            {
                MetropolisStep();
            }
        }

        public void Display()
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    Console.Write(spins[i, j] > 0 ? "+" : "-");
                }
                Console.WriteLine();
            }
        }
    }
}
// Explanation:
// Implements the Ising Model using the Metropolis-Hastings algorithm.
// Simulates spin interactions on a 2D grid.
// Dependencies:
// No external dependencies required, though Math.NET Numerics can be used for more advanced computations.