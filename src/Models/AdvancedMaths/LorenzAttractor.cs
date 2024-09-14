// Chaos Theory and Nonlinear Dynamics
// Lorenz Attractor
// File: Models/LorenzAttractor.cs
using System;

namespace Models
{
    public class LorenzAttractor
    {
        public double Sigma { get; set; }
        public double Rho { get; set; }
        public double Beta { get; set; }

        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }

        public LorenzAttractor(double sigma, double rho, double beta, double x0, double y0, double z0)
        {
            Sigma = sigma;
            Rho = rho;
            Beta = beta;
            X = x0;
            Y = y0;
            Z = z0;
        }

        public void Iterate(double dt)
        {
            double dx = Sigma * (Y - X);
            double dy = X * (Rho - Z) - Y;
            double dz = X * Y - Beta * Z;

            X += dx * dt;
            Y += dy * dt;
            Z += dz * dt;
        }

        public void Simulate(int steps, double dt)
        {
            for (int i = 0; i < steps; i++)
            {
                Iterate(dt);
                Console.WriteLine($"Step {i}: X={X}, Y={Y}, Z={Z}");
            }
        }
    }
}
// Explanation:
// This class models the Lorenz Attractor system.
// The Iterate method updates the state based on the Lorenz equations.
// The Simulate method runs the system for a specified number of steps, printing the state at each step.
// Dependencies:
// For visualization, you might consider using a plotting library like OxyPlot or LiveCharts.