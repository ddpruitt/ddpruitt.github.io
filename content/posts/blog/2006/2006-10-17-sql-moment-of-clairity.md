---
title: 'SQL Moment of Clairity'
date: 2006-10-17T12:16:00.000-07:00
draft: false
xurlx: /2006/10/sql-moment-of-clairity.html
tags: 
- SQL Origami
---

I was working on a stored procedure the other day, not one that I wrote originally but one that was causing performance issues in out system. The procedure was taking any where from 40 to 60 seconds to run, which is unacceptable in a web service. This in fact was causing the web service to time out.  
  
An inspection of the execution plan showed that one part of the procedure was doing an index scan and that it was this that was taking up over 85% of the execution time. I was totally baffled, it was hitting the index so why wasn’t it faster?  
  
Then I learned three things all at once:  
  
*   Implicit Conversion
*   non-sargable kills performance
*   Index Seek is preferred of Index Scan
  
**Implicit Conversion**  
  
In the stored proc a variable was defined as an NVARCHAR(20) but the field in the table it was being compared to was a CHAR(10). This lead to an implicit conversion of the variable to a CHAR(10), which lead into the next issue:  
  
**Non-Sargable Kills Performance**  
  
Sargable refers to the pseudo-acronym “SARG” – Search ARGument and refers to a WHERE clause that compares a constant value to a column value. The implicit conversion was causing a non-SARGable condition which means the WHERE clause cannot use an index.  
  
**Index Scan**  
  
Because of the non-sargable condition an Index Scan was being performed. An Index Scan is just as bad as a Table Scan in the SQL realm and should be avoided at all costs.  
  
The solution was simple: Change the variable to a CHAR(10). After doing that the Index Scan became an Index Seek and the whole stored procedure returned in less than a second. Any time I see an order of magnitude improvement like that from one simple change it just boggles my mind.