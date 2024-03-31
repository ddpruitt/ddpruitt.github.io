---
title: 'Simple MapReduce'
date: 2011-01-10T07:04:00.000-08:00
draft: false
xurlx: /2011/01/simple-mapreduce.html
tags: 
- .Net
---

Open file, read in lines, return individual words, get length of each word, Order by the length of the words, count each word of specific length.  
  
```csharp
  
static void Main()
{

    var counts = OpenFileReturnWords(@"LoremIpsumDolor.txt")
    .AsParallel().Select(w => w.Length)
    .AsParallel().ToLookup(k => k)
    .Select(c => new { Number = c.Key, CountOfNumber = c.Count() })
    .OrderBy(c => c.Number);

    foreach (var count in counts)
        Console.WriteLine("Count of {0:0000}: {1}", count.Number, count.CountOfNumber);

    Console.WriteLine("Total Count: {0}", counts.Sum(c => c.CountOfNumber));
}

public static IEnumerable<string> OpenFileReturnWords(string fileName)
{
    using (var reader = new StreamReader(fileName))
    {
        string line;
        while ((line = reader.ReadLine()) != null)
        {
            var wordsInLine = line.Split(new[] { ' ', '.' })
            .Where(word => !string.IsNullOrEmpty(word));

            foreach (var word in wordsInLine)
                yield return word;
        }
    }
    yield break;
}
  
```