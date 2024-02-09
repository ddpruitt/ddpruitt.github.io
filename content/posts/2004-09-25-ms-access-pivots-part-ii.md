---
title: 'MS Access Pivots Part II'
date: 2004-09-25T09:45:00.000-07:00
draft: false
url: /2004/09/ms-access-pivots-part-ii.html
tags: 
- SQL Origami
---

Pivoting a table on multiple columns with a crosstab query gets messy really fast.  To eliminate the need for complicated Multiple Value Field Queries you can use the lowly IIF() function and the obscure CHOOSE() function to pivot a table.

  
  

## Activity Table

The table Activity used in this example has the following layout:

|AcctID|DeptID|Class|Year|BeginBalance|Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|
|--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |--- |
||||||||||||||||||
  

The AcctID and DeptID refer to account and department numbers, which are a part of our fictitious company’s accounting key. The Class refers to the type of monies, in our examples we will stick to only Actual and Budget dollar amounts. The Year is the calendar year in which the values apply. The BeginBalance is the current Year’s beginning balance while the fields Jan through Dec each contain the activity for the given month.

The data in the table was generated using the following criteria:

1.  The current calendar year is 1999.
2.  The current accounting period is June.
3.  There are two years of history, so years 1997 and 1998 have actual amounts only.
4.  There is a budget for years 1999 and 2000.
5.  There are no actual amounts for the year 2000, only budget amounts.

### Budget Variance and History

In our first example we will show two techniques of producing a very common accounting scenario: generating a report for a given accounting period showing the comparison between the actual and budgeted amounts, along with history. As an added twist we will show the next years budget also. For this example we will further limit it to show January 1999 as the current accounting period.

The result we produce will need to show the account-department number, January 1997, 1998 and 1999 actual amounts, January 1999 and 2000 budget amounts.

As can be seen, we are pivoting the Actual table on two fields, Year and Class. We will employee several techniques to perform this pivoting to show that there is more than one way to do it. The techniques we will use are:

*   Select query with the IIF( ) function
*   Select query with a combination of the IIF( ) and CHOOSE( ) functions

### Pivot using IIF( ) Function

The IIF() function is a built in MS Access function. It is identical to the IF() function in Excel and has the following syntax:

```sql
      IIF( [logical test], [value if true], [value if false])
```

The logical test can be any type of expression that returns a true or false value.

As a simple example, say we wanted to select the Class field from the Activity table only the word "actual". If the Class is "actual" we want to see "Real Dollars", otherwise show "Not Real Dollars". To do this we would create the following query:

```sql
      SELECT Activity.AcctID, Activity.DeptID  
      , IIf([Class]="actual","Real Dollars","Not Real Dollars") AS RealDollars,
            Activity.BeginBalance  
      FROM Activity;
```

This would return the beginning balances with the either "Real Dollars" or "Not Real Dollars" in the RealDollars field. An example output is:

|AcctID|DeptID|RealDollars|BeginBalance|
|--- |--- |--- |--- |
|1000|1000|Real Dollars|$2,530.00|
|1000|1000|Real Dollars|$8,690.00|
|1000|1000|Real Dollars|$8,235.00|
|1000|1000|Real Dollars|$0.00|
|1000|1000|Not Real Dollars|$5,005.00|
|1000|1000|Not Real Dollars|$9,470.00|


Now let us turn our attention back to the pivot table problem. The problem is to show for the years 1997 through 1999 the actual dollar amounts (Class = actual), and for the years 1999 and 2000 the budgeted amounts (Class = budget).

Looking at the data, we know that for the years 1997 and 1998 there are no budget amounts. Because of this we can set up two fields in our query using the IIF( ) function and the table field [Year]:

```sql
      [1997] = IIF( [Year] = "1997", [Jan], 0)  
      [1998] = IIF( [Year] = "1998", [Jan], 0)
```

With these two functions the given query field value will either be the given years January actual amount or zero.

The next two years, 1999 and 2000, have both actual and budget amounts. However, since we are currently in the calendar year 1999 the year 2000 will have only budget amounts. Using this we can create fields for 1999 and 2000:

```sql
      [Actual 1999] = IIF([Year]="1999" AND [Class] = "actual", [Jan], 0)
      [Budget 1999] = IIF([Year]="1999" AND [Class] = "budget", [Jan], 0)
      [Budget 2000] = IIF([Year]="2000" AND [Class] = "budget", [Jan], 0)
```

These functions will return their indicated values, January Actual 1999, Budget 1999 or Budget 2000, or they will return a zero.

Combining these into a final select query we get:

```sql
SELECT act.AcctID, act.DeptID,
      IIf([act].[Year]="1997",[act].[Jan],0) AS 1997, 
      IIf([act].[Year]="1998",[act].[Jan],0) AS 1998, 
      IIf([act].[Year]="1999" And [act].[Class]="actual",[act].[Jan],0) AS Actual1999,
      IIf([act].[Year]="1999" And [act].[Class]="budget",[act].[Jan],0) AS Budget1999,
      IIf([act].[Year]="2000" And [act].[Class]="actual",[act].[Jan],0) AS Actual2000,
      IIf([act].[Year]="2000" And [act].[Class]="budget",[act].[Jan],0) AS Budget2000
FROM Activity AS act;
```

As a result for account 1000 we get:

|AcctID|DeptID|1997|1998|Actual1999|Budget1999|Budget2000|
|--- |--- |--- |--- |--- |--- |--- |
|1000|1000|$1,955.00|$0.00|$0.00|$0.00|$0.00|
|1000|1000|$0.00|$3,925.00|$0.00|$0.00|$0.00|
|1000|1000|$0.00|$0.00|$9,195.00|$0.00|$0.00|
|1000|1000|$0.00|$0.00|$0.00|$0.00|$0.00|
|1000|1000|$0.00|$0.00|$0.00|$3,575.00|$0.00|
|1000|1000|$0.00|$0.00|$0.00|$0.00|$8,470.00|
|1000|2000|$8,540.00|$0.00|$0.00|$0.00|$0.00|
|1000|2000|$0.00|$8,090.00|$0.00|$0.00|$0.00|
|1000|2000|$0.00|$0.00|$9,620.00|$0.00|$0.00|
|1000|2000|$0.00|$0.00|$0.00|$0.00|$0.00|
|1000|2000|$0.00|$0.00|$0.00|$9,980.00|$0.00|
|1000|2000|$0.00|$0.00|$0.00|$0.00|$6,145.00|
|1000|2010|$9,285.00|$0.00|$0.00|$0.00|$0.00|
|1000|2010|$0.00|$5,510.00|$0.00|$0.00|$0.00|
|1000|2010|$0.00|$0.00|$1,820.00|$0.00|$0.00|
|1000|2010|$0.00|$0.00|$0.00|$0.00|$0.00|
|1000|2010|$0.00|$0.00|$0.00|$2,340.00|$0.00|
|1000|2010|$0.00|$0.00|$0.00|$0.00|$5,110.00|

As you can see there are a lot of zeros. It almost looks like we accomplished the goal of the problem. We have columns with only 1997 values, or 1998 values, or Actual 1999 values, etc, which means we have successfully pivoted on the desired fields, but this result set is quit useless as it is. What we need now is a way to reduce the rows down so that true comparisons can be made between the history and the budget.

This is accomplished by using the aggregate function sum() in the query. Rewriting the query and summing on the amount columns we generate the following select query:

```sql
SELECT
      act.AcctID, 
      act.DeptID, 
      Sum(IIf([act].[Year]="1997",[act].[Jan],0)) AS 1997,
      Sum(IIf([act].[Year]="1998",[act].[Jan],0)) AS 1998, 
      
      Sum(IIf([act].[Year]="1999" 
            And[act].[Class]="actual",[act].[Jan],0)) AS Actual1999, 
      
      Sum(IIf([act].[Year]="1999" 
            And[act].[Class]="budget",[act].[Jan],0)) AS Budget1999, 
      
      Sum(IIf([act].[Year]="2000" 
            And[act].[Class]="budget",[act].[Jan],0)) AS Budget2000

FROM Activity AS act
GROUP BY act.AcctID, act.DeptID;
```

This query returns the results we can use:

|AcctID|DeptID|1997|1998|Actual1999|Budget1999|Budget2000|
|--- |--- |--- |--- |--- |--- |--- |
|1000|1000|$1,955.00|$3,925.00|$9,195.00|$3,575.00|$8,470.00|
|1000|2000|$8,540.00|$8,090.00|$9,620.00|$9,980.00|$6,145.00|
|1000|2010|$9,285.00|$5,510.00|$1,820.00|$2,340.00|$5,110.00|
|1000|2020|$8,460.00|$5,005.00|$8,825.00|$9,000.00|$9,500.00|
|1000|3000||$8,355.00|$2,265.00|$6,115.00|$5,050.00|
|1000|3010|$6,715.00|$8,665.00|$5,390.00|$695.00|$5,885.00|
|1000|4000|$540.00|$9,085.00|$7,775.00|$8,550.00|$8,255.00|
|1000|4010|$7,885.00|$1,035.00|$2,870.00|$1,265.00|$5,510.00|

By grouping on the AcctID and DeptID and summing on the other columns we have effectively pivoted the table on two columns, [Year] and [Class].

Looking at the first result set (without the sum()) we see that for AcctID 1000, DeptID 1000 there is one row for each non-zero column value (this is assuming that no original value was zero to begin with). What is not seen is the fact that this result is really a snap shot of the original table but without showing the [Year] and [Class] columns, these are hidden in the IIF( ) function.

By comparing the original table (below) to the two above we can see how the values where first pivoted on the [Year] field (first query) then summed to pivot on the [Class] field.

|AcctID|DeptID|Class|Year|Jan|
|--- |--- |--- |--- |--- |
|1000|1000|actual|1997|$1,955.00|
|1000|1000|actual|1998|$3,925.00|
|1000|1000|actual|1999|$9,195.00|
|1000|1000|actual|2000|$0.00|
|1000|1000|budget|1999|$3,575.00|
|1000|1000|budget|2000|$8,470.00|
|1000|2000|actual|1997|$8,540.00|
|1000|2000|actual|1998|$8,090.00|
|1000|2000|actual|1999|$9,620.00|
|1000|2000|actual|2000|$0.00|
|1000|2000|budget|1999|$9,980.00|
|1000|2000|budget|2000|$6,145.00|
|1000|2010|actual|1997|=right>$9,285.00|
|1000|2010|actual|1998|$5,510.00|
|1000|2010|actual|1999|$1,820.00|
|1000|2010|actual|2000|$0.00|
|1000|2010|budget|1999|$2,340.00|
|1000|2010|budget|2000|$5,110.00|


Also, note that this was a single select query as opposed to the use of two crosstabs combined in a select query.

### Pivot Using IIF( ) and CHOOSE( ) Functions

From the previous examples we see how to pivot a table on two columns. In all cases though, we were only able to show data for one accounting period, January. In order to show any of the other months activity using the above techniques we would have to create eleven more queries. Maintaining twelve queries, which only shows the data in one way is very cumbersome and over time as the number of reports needed increased, impossible to maintain.

The solution would be to write a single query with a parameter to be used in all twelve cases. The parameter would be the number of the month whose values we want to see. If we were to stay with just using the IIF( ) function our query would have fields like this:

```sql
[1997] = IIF([Year]="1999", (
            IIF([Month]=1, [JAN], (  
            IIF([Month]=2, [FEB], …0) 0)
            0)
```

This again would be impossible to maintain.

The alternative is to use the CHOOSE() function. The syntax is:

```sql
 Choose (index, choice-1[, choice-2, ... [, choice-n]])
```

Where index refers to the choice number. If index equals 1 then choice-1 is returned, if it is 2 then choice-2 is returned, on down to index equal n then choice-n is returned.

Using our parameter we can rewrite the above function as:

```sql
 [1997] = IIF([Year]="1997", Choose([Month], [JAN], [FEB], …,[DEC]), 0)
```

with all twelve months listed in the single Choose() function. This is easier to understand and maintain. When the query is executed we are asked for a month and the correct values are returned.

The select query would look like the following:

```sql
PARAMETERS MonthNum Short;
SELECT 
      act.AcctID,
      act.DeptID, 
      Sum(IIf([act].[Year]="1997", 
      Choose([MonthNum]
      ,[act].[Jan],[act].[Feb],[act].[Mar] ,[act].[Apr],[act].[May],[act].[Jun]
      ,[act].[Jul],[act].[Aug],[act].[Sep] ,[act].[Oct],[act].[Nov],[act].[Dec]),0)) AS 1997,
      
      Sum(IIf([act].[Year]="1998", 
            Choose([MonthNum] ,[act].[Jan],[act].[Feb],[act].[Mar]
            ,[act].[Apr],[act].[May],[act].[Jun] ,[act].[Jul],[act].[Aug],[act].[Sep]
            ,[act].[Oct],[act].[Nov],[act].[Dec]),0)) AS 1998, 
      
      Sum(IIf([act].[Year]="1999" And [act].[Class]="actual", 
            Choose([MonthNum] ,[act].[Jan],[act].[Feb],[act].[Mar]
            ,[act].[Apr],[act].[May],[act].[Jun] ,[act].[Jul],[act].[Aug],[act].[Sep]
            ,[act].[Oct],[act].[Nov],[act].[Dec]),0)) AS Actual1999, 
            
      Sum(IIf([act].[Year]="1999" And [act].[Class]="budget", 
            Choose([MonthNum] ,[act].[Jan],[act].[Feb],[act].[Mar]
            ,[act].[Apr],[act].[May],[act].[Jun] ,[act].[Jul],[act].[Aug],[act].[Sep]
            ,[act].[Oct],[act].[Nov],[act].[Dec]),0)) AS Budget1999, 
            
      Sum(IIf([act].[Year]="2000" And [act].[Class]="budget", 
            Choose([MonthNum] ,[act].[Jan],[act].[Feb],[act].[Mar]
            ,[act].[Apr],[act].[May],[act].[Jun] ,[act].[Jul],[act].[Aug],[act].[Sep]
            ,[act].[Oct],[act].[Nov],[act].[Dec]),0)) AS Budget2000
FROM Activity AS act
GROUP BY act.AcctID, act.DeptID;
```

Note that we still need to use the sum() aggregate function for the query to return the correct results. When we execute the query we are asked for the parameter value for MonthNum. The results for a MonthNum of 1 (January) are:

|AcctID|DeptID|1997|1998|Actual1999|Budget1999|Budget2000|
|--- |--- |--- |--- |--- |--- |--- |
|1000|1000|$1,955.00|$3,925.00|$9,195.00|$3,575.00|$8,470.00|
|1000|2000|$8,540.00|$8,090.00|$9,620.00|$9,980.00|$6,145.00|
|1000|2010|$9,285.00|$5,510.00|$1,820.00|$2,340.00|$5,110.00|
|1000|2020|$8,460.00|$5,005.00|$8,825.00|$9,000.00||
|1000|3000|$4,605.00|$8,355.00|$2,265.00|$6,115.00|$5,050.00|
|1000|3010|$6,715.00|$8,665.00|$5,390.00|$695.00|$5,885.00|
|1000|4000|$540.00|$9,085.00|$7,775.00|$8,550.00|$8,255.00|
|1000|4010|$7,885.00|$1,035.00|$2,870.00|$1,265.00|$5,510.00|


This result set matches the previous results as expected. If they had not then we would have known that this technique would not work. And if we need to see the values for any other month we would enter that month number into the parameter.