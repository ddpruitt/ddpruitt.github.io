---
title: 'MS Access Pivots Part I'
date: 2004-09-25T09:35:00.000-07:00
draft: false
url: /2004/09/ms-access-pivots-part-i.html
tags: 
- SQL Origami
---

CrossTab queries are an easy way in MS Access to pivot tables. They are limited in that you can only pivot on one field and can only aggregate one field.

  
  

As an example we look at Activity table in the Sample.mdb. If we were asked to show the January activity for each account - department combination for each year using a crosstab query, we would create the following SQL:

```sql 
   TRANSFORM Sum(Activity.Jan) AS SumOfJan  
   SELECT Activity.AcctID, Activity.DeptID  
   FROM Activity  
   WHERE (Activity.Class = "actual")  
   GROUP BY Activity.AcctID, Activity.DeptID  
   PIVOT Activity.Year In (1997,1998,1999,2000);  
```

The TRANSFORM identifies the column to aggregate and the type of aggregation (sum). Other types of aggregation possible are avg, min, max, count, etc.

The PIVOT identifies which column is pivoted on. In this case it is the Year column, the IN clause indicates that we only want to see these four specific column headings. If the IN clause was not specified then all possible values of Year would be used as column headings.

In the Query Builder, the Row Headings indicate which fields to show in the results, while the Column Heading shows the pivot field and the Value is the aggregated field. For a crosstab there is no limit to the number of Row Headings but you need one Column Heading and one Value. Of course with a crosstab you can have only one Column Heading and one Value.

The first eight rows of the result set would be:

|AcctID|DeptID|1997|1998|1999|2000|
|--- |--- |--- |--- |--- |--- |
|1000|1000|$1,955.00|$3,925.00|$9,195.00|$0.00|
|1000|2000|$8,540.00|$8,090.00|$9,620.00|$0.00|
|1000|2010|$9,285.00|$5,510.00|$1,820.00|$0.00|
|1000|2020|$8,460.00|$5,005.00|$8,825.00|$0.00|
|1000|3000|$4,605.00|$8,355.00|$2,265.00|$0.00|
|1000|3010|$6,715.00|$8,665.00|$5,390.00|$0.00|
|1000|4000|$540.00|$9,085.00|$7,775.00|$0.00|
|1000|4010|$7,885.00|$1,035.00|$2,870.00|$0.00|

As can be seen this is a very simple query, easy to setup and understand. Simply identify the column to pivot on and what column to aggregate and that is it. The problems start to arise whenever more than one pivot value or transform column is needed.

## Pivoting on Two Columns - Multiple Value Field Query

Single CrossTab queries cannot pivot on more than one column. However, situations arise frequently where it is needed to pivot on two or more columns. One workaround for this, as documented in the Microsoft Knowledge Base (Q109939), is to build separate crosstab queries using the same underlying table, each pivoting on the same column but showing only specific values from the second column. The final result is obtained by joining the crosstab queries in a Select query. Microsoft refers to this as a **Multiple Value Field** query.

This method of joining crosstab queries is simple to implement whenever the number of columns to pivot on is small. However, as the number of column pivots increases the number of required joins also increases. Eventually a point is reached where the query is too complex to maintain or even to complex to execute. A rule of thumb should be that if it is needed to pivot on more than two columns then crosstab queries should not be used.

To create the Budget Variance and History report with a crosstab we will need to use a Multiple Value Field query. To accomplish this we use the previous crosstab query for our actual amounts and create the following crosstab for our budget amounts:

```sql
   TRANSFORM Sum(Activity.Jan) AS SumOfJan  
   SELECT Activity.AcctID, Activity.DeptID  
   FROM Activity  
   WHERE (Activity.Class = "budget")  
   GROUP BY Activity.AcctID, Activity.DeptID  
   PIVOT Activity.Year In (1997,1998,1999,2000);  
```

As you can see the only difference is the WHERE clause.

Next we create the select query using both of the crosstab queries as the source. We join on the account and department numbers:

```sql
   SELECT  
      actual.AcctID, actual.DeptID,  
  
      actual.[1997], actual.[1998],  
      actual.[1999], budget.[1999]  
      actual.[2000], budget.[2000]  
   FROM  
      [CrossTab Actual by Year] AS actual  
      INNER JOIN [CrossTab Budget by Year] AS budget  
         ON (actual.DeptID = budget.DeptID)  
         AND (actual.AcctID = budget.AcctID);  
```

In this case since we are currently in the year 1999 we would not have budget numbers for 1997 or 1998. The query builder looks like:

The results from this are:

|AcctID|DeptID|1997|1998|actual.1999|Budget.1999|actual.2000|budget.2000|
|--- |--- |--- |--- |--- |--- |--- |--- |
|1000|1000|$1,955.00|$3,925.00|$9,195.00|$3,575.00|$0.00|$8,470.00|
|1000|2000|$8,540.00|$8,090.00|$9,620.00|$9,980.00|$0.00|$6,145.00|
|1000|2010|$9,285.00|$5,510.00|$1,820.00|$2,340.00|$0.00|$5,110.00|
|1000|2020|$8,460.00|$5,005.00|$8,825.00|$9,000.00|$0.00|$9,500.00|
|1000|3000|$4,605.00|$8,355.00|$2,265.00|$6,115.00|$0.00|$5,050.00|
|1000|3010|$6,715.00|$8,665.00|$5,390.00|$695.00|$0.00|$5,885.00|
|1000|4000|$540.00|$9,085.00|$7,775.00|$8,550.00|$0.00|$8,255.00|
|1000|4010|$7,885.00|$1,035.00|$2,870.00|$1,265.00|$0.00|$5,510.00|


From this result set it is easy to directly compare the actual amounts to the budgeted amounts. Of course it must be remembered that these amounts are from just one month and as can be guessed trying to compare two separate months from the same year using a crosstab in this manner is cumbersome.

Using crosstabs for simple queries is easy. However for typical reporting needs crosstabs become too unwieldy and difficult to maintain. Fortunately we do have some alternatives.