---
title: 'Scratching Parallel with StopWatch'
date: 2011-01-18T00:54:00.000-08:00
draft: false
xurlx: /2011/01/scratching-parallel-with-stopwatch.html
tags: 
- .Net
---

Threw together a quick parallel stopwatch test. Not sure if the times prove anything.  
  
```csharp 
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace Scratch.ParallelProcessing
{
    class Program
    {
        static void Main(string[] args)
        {
            const int count = 10000000;
            var source1 = Enumerable.Range(0, count).ToArray();
            var source2 = Enumerable.Range(0, count).ToArray();
            var source3 = Enumerable.Range(0, count).ToArray();

            Stopwatch stopwatch = new Stopwatch();

            var parallelElapsedTimes = new List<TimeSpan>();
            var linearElapsedTimes = new List<TimeSpan>();
            var linqSelectElapsedTimes = new List<TimeSpan>();

            for (int i = 0; i < 10; i++)
            {
                stopwatch.Reset();
                stopwatch.Start();
                var parallelResults = Parallel.ForEach(source1, s => s %= 2);
                parallelElapsedTimes.Add(stopwatch.Elapsed);
                stopwatch.Reset();

                stopwatch.Start();
                LinearAction(source2, s => s %= 2);
                linearElapsedTimes.Add(stopwatch.Elapsed);
                stopwatch.Reset();

                stopwatch.Reset();
                stopwatch.Start();
                Array.ForEach(source3, s => s = s % 2);
                linqSelectElapsedTimes.Add(stopwatch.Elapsed);
                stopwatch.Reset();

            }

            Console.WriteLine("Elapsed TimettMinttMaxtttAvg");
            Console.WriteLine("============tt===tt===ttt===");
            Console.WriteLine("{0}tt{1}tt{2}ttt{3}", "Parallel", parallelElapsedTimes.Min(t => t.Milliseconds), parallelElapsedTimes.Max(t => t.Milliseconds), parallelElapsedTimes.Average(t => t.Milliseconds));
            Console.WriteLine("{0}ttt{1}tt{2}ttt{3}", "Linear", linearElapsedTimes.Min(t => t.Milliseconds), linearElapsedTimes.Max(t => t.Milliseconds), linearElapsedTimes.Average(t => t.Milliseconds));
            Console.WriteLine("{0}ttt{1}tt{2}ttt{3}", "Linq", linqSelectElapsedTimes.Min(t => t.Milliseconds), linqSelectElapsedTimes.Max(t => t.Milliseconds), linqSelectElapsedTimes.Average(t => t.Milliseconds));

        }

        public static void LinearAction<T>(IEnumerable<T> source, Action<T> action)
        {
            foreach (var s in source) action(s);
        }
    }
}
```  
  
Results of the timer: 
``` 
`  
Elapsed Time Min Max Avg  
============ === === ===  
Parallel 63 191 79.5  
Linear 138 143 140.3  
Linq 54 56 54.5  
Press any key to continue . . .  
`  
```

I'm running 64 bit Vista on a Intel Core2 Duo with 4GB RAM. The Parallel seems to be inconsistent, and depends a lot on whether or not it grabs that second CPU.