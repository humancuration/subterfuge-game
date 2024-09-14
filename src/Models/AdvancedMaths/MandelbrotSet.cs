// Fractal Geometry
// Mandelbrot Set
// File: Models/MandelbrotSet.cs
using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace Models
{
    public class MandelbrotSet
    {
        private int width;
        private int height;
        private int maxIterations;

        public MandelbrotSet(int width, int height, int maxIterations)
        {
            this.width = width;
            this.height = height;
            this.maxIterations = maxIterations;
        }

        public Bitmap Generate()
        {
            Bitmap bmp = new Bitmap(width, height);
            for (int px = 0; px < width; px++)
            {
                for (int py = 0; py < height; py++)
                {
                    double x0 = (px / (double)width) * 3.5 - 2.5;
                    double y0 = (py / (double)height) * 2.0 - 1.0;
                    double x = 0.0;
                    double y = 0.0;
                    int iteration = 0;

                    while (x * x + y * y <= 4.0 && iteration < maxIterations)
                    {
                        double xtemp = x * x - y * y + x0;
                        y = 2 * x * y + y0;
                        x = xtemp;
                        iteration++;
                    }

                    Color color = iteration == maxIterations ? Color.Black : Color.FromArgb(255, iteration % 256, (iteration * 5) % 256, (iteration * 15) % 256);
                    bmp.SetPixel(px, py, color);
                }
            }
            return bmp;
        }

        public void SaveImage(string path)
        {
            Bitmap bmp = Generate();
            bmp.Save(path, ImageFormat.Png);
        }
    }
}
// Explanation:
// This class generates an image of the Mandelbrot Set.
// The Generate method computes the set and colors each pixel based on the number of iterations.
// The SaveImage method saves the generated image to a file.
// Dependencies:
// Utilizes System.Drawing for image manipulation. Ensure your project references System.Drawing.Common.