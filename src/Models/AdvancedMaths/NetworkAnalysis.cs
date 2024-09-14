// Complex Network Theory
// Adjacency Matrices and Eigenvector Centrality
// File: Models/NetworkAnalysis.cs
using System;
using MathNet.Numerics.LinearAlgebra;

namespace Models
{
    public class NetworkAnalysis
    {
        public Matrix<double> AdjacencyMatrix { get; set; }

        public NetworkAnalysis(Matrix<double> adjacencyMatrix)
        {
            AdjacencyMatrix = adjacencyMatrix;
        }

        public Vector<double> ComputeEigenvectorCentrality()
        {
            var eigen = AdjacencyMatrix.Evd();
            var leadingEigen = eigen.EigenValues.AbsoluteMaximum();
            var index = eigen.EigenValues.AbsoluteMaximumIndex();
            var centrality = eigen.EigenVectors.Column(index);
            return centrality;
        }
    }
}
// Explanation:
// Calculates the eigenvector centrality of a network based on its adjacency matrix.
// Uses Math.NET Numerics for eigen decomposition.
// Dependencies:
// Math.NET Numerics: Install via NuGet with Install-Package MathNet.Numerics.