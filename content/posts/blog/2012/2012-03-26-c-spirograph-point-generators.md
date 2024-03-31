---
title: 'C# Spirograph Point Generators'
date: 2012-03-26T01:02:00.000-07:00
draft: false
xurlx: /2012/03/c-spirograph-point-generators.html
tags: 
- .Net
- graphic
- Spirograph
- c#
- math
---

[Spirograph's](http://en.wikipedia.org/wiki/Spirograph "Spirograph's")  are cool.  See [here](http://www.math.psu.edu/dlittle/java/parametricequations/spirograph/SpiroGraph1.0/index.html "here") and [here](http://www.math.psu.edu/dlittle/java/parametricequations/spirograph/index.html "here").  
  
![](http://upload.wikimedia.org/wikipedia/commons/9/90/Various_Spirograph_Designs.jpg "Various Spirograph Designs")  
  
I put together three ways to generate points for a Spirograph, first using a Brute Force straight generate the points, second using a Parallel.For and third using LINQ.  
  
  
  
Two important classes first.  First is a static DoubleExtension class to help determine when two doubles are about equal:  
  
```csharp  
using System;

namespace DC.SpiroGraph.Core
{
    public static class DoubleExtension
    {
        public static bool AboutEqual(double x, double y)
        {
            double epsilon = Math.Max(Math.Abs(x), Math.Abs(y)) * 1E-15;

            var variance = x > y ? x - y : y - x;
            return Math.Abs(variance)
        }
    }
}
```  
  
Second, a simple Point class. Yeah, I probably could have used the Drawing Point class.  
  
```csharp  
using System;  
using System.Diagnostics;  
  
namespace DC.SpiroGraph.Core  
{  
[DebuggerDisplay("X = {X}, Y = {Y}")]  
public class Point  
{  
public double X { get; set; }  
public double Y { get; set; }  
  
public bool NotAboutEqualTo(Point point)  
{  
return !DoubleExtension.AboutEqual(X, point.X) && !DoubleExtension.AboutEqual(Y, point.Y);  
}  
}  
}  
```  
  
Note the use of DebuggerDisplay to help with the debugging.  
The SpiroGraphGenerator:  
  
```csharp  
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace DC.SpiroGraph.Core
{
    public class SpiroGraphGenerator
    {
        /// <summary>  
        /// Radius, R, of Circle (equator) centered at the origin  
        /// </summary>  
        public double Radius1 { get; set; }

        /// <summary>  
        /// Radius, r, of circle (bicyle wheel) cetered at (R + r, 0)  
        /// </summary>  
        public double Radius2 { get; set; }

        /// <summary>  
        /// Distance of Point (reflector) from the center of Circle 2 (the circle of Radius2)  
        /// </summary>  
        public double Position { get; set; }

        /// <summary>  
        /// Controls how precise the SpiroGraph is drawn.  
        /// Controls t of f(t).  
        /// Resolution = 360, t = 0, 1, 2, 3, ...,360  
        /// Resolution = 180, t = 0, 2, 4, 6, ... 360  
        ///  
        /// </summary>  
        public double Resolution { get; set; }

        public IEnumerable<Point> GetSpiroGraphPoints()
        {

            var t = 0d;
            var sumOfRadius = Radius1 + Radius2;

            var firstPoint = new Point { X = Xt(sumOfRadius, Radius2, Position, t), Y = Yt(sumOfRadius, Radius2, Position, t) };
            var currentPoint = new Point { X = firstPoint.X, Y = firstPoint.Y };

            // Convert to Radians  
            var increment = (360 / Resolution) * Math.PI / 180;

            do
            {
                for (var i = 0; i < Resolution; i++)
                {
                    yield return currentPoint;
                    t += increment;
                    currentPoint = new Point { X = Xt(sumOfRadius, Radius2, Position, t), Y = Yt(sumOfRadius, Radius2, Position, t) };
                }
            } while (currentPoint.NotAboutEqualTo(firstPoint));

            yield return currentPoint;
        }

        public IEnumerable<Point> GetSpiroGraphPoints2()
        {
            var endPoints = FindAllEndPoints();
            var numberOfEndPonits = endPoints.Count();
            var numberOfPoints = Convert.ToInt32((numberOfEndPonits - 1) * Resolution + 1);
            var points = new Point[numberOfPoints];
            var resolution = Convert.ToInt32(Resolution);

            var sumOfRadius = Radius1 + Radius2;

            // Convert to Radians  
            var increment = (360 / Resolution) * Math.PI / 180;
            points[0] = GetPoint(sumOfRadius, 0);

            Parallel.For(1, numberOfEndPonits, i =>
            {
                for (var j = 0; j < resolution; j++)
                {
                    var pointIndex = (i - 1) * resolution + j + 1;
                    var t = pointIndex * increment;
                    points[pointIndex] = GetPoint(sumOfRadius, t);
                }
            });
            return points;
        }

        public IEnumerable<Point> GetSpiroGraphPoints3()
        {
            var endPoints = FindAllEndPoints();
            var numberOfEndPonits = endPoints.Count();
            var numberOfPoints = Convert.ToInt32((numberOfEndPonits - 1) * Resolution + 1);

            var sumOfRadius = Radius1 + Radius2;

            // Convert to Radians  
            var increment = (360 / Resolution) * Math.PI / 180;

            var points2 = Enumerable.Range(0, numberOfPoints)
            .Select(i => new { Index = i, t = i * increment })
            .Select(it => new { it.Index, point = GetPoint(sumOfRadius, it.t) })
            .OrderBy(ip => ip.Index)
            .Select(ip => ip.point);

            return points2;
        }

        public IEnumerable<Point> FindAllEndPoints()
        {
            var t = 0d;
            var sumOfRadius = CalculateSumOfRadius();

            var firstPoint = GetPoint(sumOfRadius, t);
            var currentPoint = new Point { X = firstPoint.X, Y = firstPoint.Y };
            // Convert to Radians  
            var increment = CalculateIncrement();

            do
            {
                yield return currentPoint;
                t += (increment * Resolution);
                currentPoint = GetPoint(sumOfRadius, t);
            } while (currentPoint.NotAboutEqualTo(firstPoint));

            yield return currentPoint;
        }

        private double CalculateIncrement()
        {
            return (360 / Resolution) * Math.PI / 180;
        }

        private double CalculateSumOfRadius()
        {
            return Radius1 + Radius2;
        }

        private static double Xt(double sumOfRadius, double radius2, double position, double t)
        {
            return sumOfRadius * Math.Cos(t) + position * Math.Cos(sumOfRadius * t / radius2);
        }

        private static double Yt(double sumOfRadius, double radius2, double position, double t)
        {
            return sumOfRadius * Math.Sin(t) + position * Math.Sin(sumOfRadius * t / radius2);
        }

        private Point GetPoint(double sumOfRadius, double t)
        {
            return new Point { X = Xt(sumOfRadius, Radius2, Position, t), Y = Yt(sumOfRadius, Radius2, Position, t) };
        }
    }
}
```  
  
I created a simple WinForm app using DevExpress controls, this is how I populated the graph with points:  
  
```csharp 
using System;
using System.Linq;
using System.Windows.Forms;

namespace DC.SpiroGraph.WinForm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public void CreateData(bool useParallel)
        {
            try
            {
                Cursor.Current = Cursors.WaitCursor;

                var sg = new Core.SpiroGraphGenerator
                {
                    Radius1 = Convert.ToDouble(radius1TextEdit.Text),
                    Radius2 = Convert.ToDouble(radius2TextEdit.Text),
                    Position = Convert.ToDouble(positionTextEdit.Text),
                    Resolution = Convert.ToDouble(resolutionTextEdit.Text)
                };

                var endPoints = sg.FindAllEndPoints();
                endPointCountLabel.Text = string.Format("# of End Points: {0}", endPoints.Count());

                var graphPoints = useParallel ? sg.GetSpiroGraphPoints2() : sg.GetSpiroGraphPoints3();
                pointCountLabel.Text = string.Format("# of Points: {0}", graphPoints.Count());

                chartControl1.Series[0].DataSource = graphPoints;

            }
            catch (Exception)
            {

                // Eat It  
            }
            finally
            {
                Cursor.Current = Cursors.Default;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            var series = chartControl1.Series[0];
            series.Label.Visible = false;
            series.ArgumentDataMember = "X";
            series.ValueDataMembers.AddRange(new string[] { "Y" });

            radius1TextEdit.Text = "60";
            radius2TextEdit.Text = "60";
            positionTextEdit.Text = "60";
            resolutionTextEdit.Text = "270";

            CreateData(true);
        }

        private void simpleButton1_Click(object sender, EventArgs e)
        {
            CreateData(true);
        }

        private void simpleButton2_Click(object sender, EventArgs e)
        {
            CreateData(false);
        }
    }
}
``` 
  
Future changes will be to make a series for each rotation about the main circle, this way we can start adding different colors to the graph.