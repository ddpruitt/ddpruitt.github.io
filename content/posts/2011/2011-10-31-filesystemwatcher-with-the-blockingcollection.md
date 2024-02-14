---
title: 'FileSystemWatcher With the BlockingCollection'
date: 2011-10-31T02:48:00.000-07:00
draft: false
xurlx: /2011/10/filesystemwatcher-with.html
tags: 
- .Net
---

While working with the FileSystemWatcher I found that if too many files were created the built in buffer will overflowed and files will be skipped.  After much research I found out about the [Producer-Consumer Problem](http://en.wikipedia.org/wiki/Producer-consumer_problem "Producer-Consumer Problem").  Then I found that .Net 4 has the BlockingCollection which helps solve the issue.  But how to use it with the FileSystemWatcher?  
  
On StackOverflow I found [Making PLINQ and BlockingCollection work together](http://stackoverflow.com/questions/7533067/making-plinq-and-blockingcollection-work-together "Making PLINQ and
      BlockingCollection work together").  I'm not so interested in the PLINQ issue but this is a great example of using The BlockingCollection with FileSystemWatcher.  
  
```csharp
using System;  
using System.Collections.Concurrent;  
using System.Collections.Generic;  
using System.IO;  
using System.Linq;  
using System.Threading;  
  
namespace ConsoleApplication4  
{  
    public class Program  
    {  
        private const string Folder = "C:TempInputData";  
  
        static void Main(string[] args) {  
  
            var cts = new CancellationTokenSource();  
            foreach (var obj in Input(cts.Token))  
                Console.WriteLine(obj);  
        }  
  
        public static IEnumerable<object> Input(CancellationToken cancellationToken) {  
            var fileList = new BlockingCollection<string>();  
  
            var watcher = new FileSystemWatcher(Folder);  
            watcher.Created += (source, e) => {  
                if (cancellationToken.IsCancellationRequested)  
                    watcher.EnableRaisingEvents = false;  
                else if (Path.GetFileName(e.FullPath) == "STOP") {  
                    watcher.EnableRaisingEvents = false;  
                    fileList.CompleteAdding();  
                    File.Delete(e.FullPath);  
                } else  
                    fileList.Add(e.FullPath);  
            };  
            watcher.EnableRaisingEvents = true;  
  
            return from file in  
                       fileList  
                            .GetConsumingEnumerable(cancellationToken)  
                            .AsParallel()  
                            .WithMergeOptions(ParallelMergeOptions.NotBuffered)  
                            .WithCancellation(cancellationToken)  
                            .WithDegreeOfParallelism(5)  
                   let obj = CreateMyObject(file)  
                   select obj;  
        }  
  
        private static object CreateMyObject(string file) {  
            return file;  
        }  
    }  
}  
```