---
title: 'LINQ to SQL Roundtrips: SQL Trace'
date: 2009-03-27T12:13:00.000-07:00
draft: false
url: /2009/03/linq-to-sql-roundtrips-sql-trace.html
tags:
- .Net
- SQL Origami
---

I was looking into reducing the number of database round trips that LINQ to SQL took and found an article by [David Hayden](http://davidhayden.com/blog/dave/archive/2007/08/05/LINQToSQLPerformanceTradeoffsLessDatabaseRoundtripsButDuplicatedData.aspx "David Hayden") that fit the bill. I wanted to see what was actually happening so I slapped together a simple demo.

Using the Pubs database I created a console app, added the LINQ to SQL classes then created a simple repository class:

```csharp
public class AuthorRepository
{
      public author GetAuthorWithTitles(string authorId)
      {
            var db = new PubsDataClassesDataContext();
            return db.authors.FirstOrDefault(a => a.au_id == authorId);
      }

      public author GetAuthorWithTitlesWithUsing(string authorId)
      {
            using (var db = new PubsDataClassesDataContext())
            return db.authors.FirstOrDefault(a => a.au_id == authorId);
      }

      public author GetAuthorWithTitlesPrefetch(string authorId)
      {

            using (var db = new PubsDataClassesDataContext())
            {
                  var options = new DataLoadOptions();

                  options.LoadWith<author>(a => a.titleauthors);

                  options.LoadWith<titleauthor>(ta => ta.title);
                  db.LoadOptions = options;

                  return db.authors.FirstOrDefault(a => a.au_id == authorId);
            }
      }
}
```

I created a simple method to dump the author and titles:

```csharp
class Program
{
     static void Main(string[] args)
     {
          var repo = new AuthorRepository();

          DumpAuthorToConsole(
               repo.GetAuthorWithTitles("998-72-3567")
               , "Authors without prefetch and without using statement");

          DumpAuthorToConsole(
               repo.GetAuthorWithTitlesWithUsing("998-72-3567")
               , "Authors without prefetch and but with using statement");

          DumpAuthorToConsole(
               repo.GetAuthorWithTitlesPrefetch("998-72-3567")
          , "Authors with prefetch and with using statement");
 }
     private static void DumpAuthorToConsole(author author, string message)
     {
          Console.WriteLine();
          Console.WriteLine(new string('-', 50));
          Console.WriteLine(message);
          try
          {
               Console.WriteLine("Author Name: { 0}{ 1}"
                                    , author.au_fname, author.au_lname);

               Console.WriteLine("Count of Titles: { 0}", author.titleauthors.Count);

               foreach (var titleauthor in author.titleauthors)
                    Console.WriteLine("tBook Title: {0}", titleauthor.title.title1);
          }
          catch (Exception e)
          {
               Console.WriteLine(">>> FAIL!<<< ");
               Console.WriteLine(e.Message);
          }
     }
}
```

I did cheat and added some titles to the chosen author just so I could have at least five titles returned. The results are:


```
Authors without prefetch and without using statement
Author Name: Albert Ringer
Count of Titles: 5
Book Title: Silicon Valley Gastronomic Treats
Book Title: Secrets of Silicon Valley
Book Title: Computer Phobic AND Non-Phobic Individuals: Behavior Variations
Book Title: Is Anger the Enemy?
Book Title: Life Without Fear
```

```
Authors without prefetch and but with using statement
Author Name: Albert Ringer
>>> FAIL! <<<
Cannot access a disposed object.
Object name: 'DataContext accessed after Dispose.'.
```

```
Authors with prefetch and with using statement
Author Name: Albert Ringer
Count of Titles: 5
Book Title: Silicon Valley Gastronomic Treats
Book Title: Secrets of Silicon Valley
Book Title: Computer Phobic AND Non-Phobic Individuals: Behavior Variations
Book Title: Is Anger the Enemy?
Book Title: Life Without Fear
Press any key to continue . . .
```

Notice that there are three cases above, with one failing.

## What are we looking at?

### Case 1: Authors without prefetch and without using statement

This is the GetAuthorsWithTitles() method, it simple creates the DataContext and returns the author object. Simple, clean and easy, but not very effecient.

First note that the DataContext was not disposed so it is still lingering out there, waiting to be garbage collected. The SQL trace looks like this:

[![](/assets/techshorts/case01.png)](/assets/techshorts/case01.png)


The first sp_executesql loads the author, the second loads all the titleauthor rows for the author and the rest select each individual title from the titles table. So for one author, a count of his titles and a list of each title requiers seven round trips to the database. Notice that each time a round trip is made a connection has to be created.


### Case 2: Authors without prefetch and but with using statement

This is the GetAuthorsWithTitleWithUsing() method, which is the same as before in that it simple creates a DataContext and returns the author but then properly disposes of the context. This fails to return the count of the titles and the individual titles because the lazy loading cannot happen on a disposed context so an exception is thrown in this case.

The trace only shows one sp_executesql:
[![](/assets/techshorts/case02.png)](/assets/techshorts/case02.png)


### Case 3: Authors with prefetch and with using statement

In this case we use the DataL\oadOptions to tell LINQ to load the titleauthors with the author and to load the titles with the titleauthors. The DataContext is disposed after returning the author.

[![](/assets/techshorts/case03.png)](/assets/techshorts/case03.png)

Notice that two sp_executesql's were executed and that both were on the same connection. The first sql returned the author:

```sql
exec sp_executesql
    N'SELECT TOP (1) [t0].[au_id]
        , [t0].[au_lname]
        , [t0].[au_fname]
        , [t0].[phone]
        , [t0].[address]
        , [t0].[city]
        , [t0].[state]
        , [t0].[zip]
        , [t0].[contract]
    FROM [dbo].[authors] AS [t0]
    WHERE [t0].[au_id] = @p0'
    ,N'@p0 varchar(11)',@p0='998-72-3567'
```

the second returned the titleauthor and titles:

```sql
exec sp_executesql
     N'SELECT [t0].[au_id]
          , [t0].[title_id]
          , [t0].[au_ord]
          , [t0].[royaltyper]
          , [t1].[title_id] AS [title_id2]
          , [t1].[title] AS [title1]
          , [t1].[type]
          , [t1].[pub_id]
          , [t1].[price]
          , [t1].[advance]
          , [t1].[royalty]
          , [t1].[ytd_sales]
          , [t1].[notes]
          , [t1].[pubdate]
     FROM [dbo].[titleauthor] AS [t0]
          INNER JOIN [dbo].[titles] AS [t1] ON [t1].[title_id] = [t0].[title_id]
     WHERE [t0].[au_id] = @x1',
     N'@x1 varchar(11)',@x1='998-72-3567'
```


## Lessons Learned

First beware of disposing the DataContext when you want to LazyLoad entities. If you are going to do this then come up with a way to properly dispose of the context.

Second round trips to the database can be reduced by using the DataLoadOptions. This is not a guaranty of better performance but it is a step in the right direction.