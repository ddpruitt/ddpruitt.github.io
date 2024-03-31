---
title: 'DataSet: More reasons to not like it'
date: 2009-02-25T07:35:00.000-08:00
draft: false
xurlx: /2009/02/dataset-more-reasons-to-not-like-it.html
tags: 
- .Net
- Rant
---

So I am still living in the world of DataSets, when will I ever learn?  
  
The task was simple, take the contents of DataSetA and merge it into DataSetB. Sure, just use:  

```csharp  
DataSetA.Merge(DataSetB, true)  
```
  
and life will be good. But wait! Why is it when I try to save the merged data from DataSetB to the database it doesn't show up?  
  
Because the RowState of all the rows in all the tables are set to Unchanged! And a merge operation does not change the RowState. So if I want all the data from B to be saved to A I have to change all the row states of all the tables to Added. Sure, I should be able to loop thorough it all and set the row's state as such:  
  
```csharp
row.RowState = DataRowState.Added;  
```
  
Not so fast grasshopper! row.RowState does not have a setter! Isn't that Asinine! You have to use:  
  
```csharp
row.SetAdded();  
```
  
What ever, I say setter, you say method. Long story short I created an extension method that works.  

```csharp  
public static DataSet MergerAllDataAsNew(this DataSet target, DataSet source)  
{  
    foreach (DataTable table in source.Tables)  
        foreach (DataRow row in table.Rows)  
            row.SetAdded();  
        
    target.Merge(source, true);  
    
    return target;  
}  
```

Brute Force works.