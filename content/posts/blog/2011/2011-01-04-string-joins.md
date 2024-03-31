---
title: 'String Joins'
date: 2011-01-04T08:27:00.000-08:00
draft: false
xurlx: /2011/01/string-joins.html
tags: 
- .Net
---

I've seen a lot of code to generate SQL statements. Invariable the programmer has an array of strings that they loop through (for example to put into an IN clause) and they always have a check to see of the current item is the first or last in the list. The typical usage is to have a **StringBuilder** and an **if** statement which determines if an extra comma (or plus sign or whatever) is added or left out.  
  
I say: **Stop Doing That!**  
  
Use the string.Join().  
  
```csharp 
  
var strings = new[] { "Darren", "Dawn", "Thomas", "Zoey" };  
  
var results = string.Format("Replace "{0}" with {1} Question Marks: ({2})",  
string.Join(",", strings), strings.Length,  
string.Join(",", Enumerable.Repeat("?", strings.Length))  
);  
  
Console.WriteLine(results);  
  
```  
  
The resulting output is:  

```  
`Replace "Darren,Dawn,Thomas,Zoey" with 4 Question Marks: (?,?,?,?)`
```