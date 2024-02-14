---
title: 'DataTable: Finding Differences in Column Values'
date: 2009-03-13T10:56:00.000-07:00
draft: false
url: /2009/03/datatable-finding-differences-in-column.html
tags: 
- .Net
---

I have two tables in a typed DataSet and I want to compare one column in each table to see if TableA has values that are not in TableB.  
  

```csharp
IEnumerable<string> valuesInA =
      typedDataSet.TableA.AsEnumerable().Select(row => row.AField);

IEnumerable<string> valuesInB = typedDataSet.TableB.AsEnumerable().Select(row
      => row.BField);
```

```csharp
foreach (var notInB in valuesInA.Except(valuesInB))

    Debug.WriteLine(string.Format("Value not in TableB.BField: {0}", notInB));
```
  
This is assuming that both AField and BField are the same type.