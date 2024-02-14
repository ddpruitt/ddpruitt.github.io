---
title: 'ExcelPackage'
date: 2007-10-19T02:02:00.000-07:00
draft: false
xurlx: /2007/10/excelpackage.html
tags: 
- General
---

I've been reviewing the [ExcelPackage](http://www.codeplex.com/ExcelPackage "ExcelPackage") project on CodePlex. It is a useful project but one thing I was wondering is why does it directly interact with the XML?  
  
I think it is a performance and a code maintenance hit to work directly with the XML for the workbook and worksheets. In my oh so humble opinion I think it would be easier to create plain old C# objects to work with the cells, rows, sheets, etc. and then serialize and de-serialize as needed. I say this because in the XML for a worksheet the cells are stored in XML nodes that have the row number and individual cell addresses in them:  
  
\[xml\]  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
\[/xml\]  
  
So if we work directly with the XML what has to happen when we insert a row? We have to loop through all of the XML nodes and update the row values. Note that I am not even talking about how to deal with merge cells or ranges (which the ExcelPackage does not work with).  
  
I think that creating a [Attribute-value system](http://en.wikipedia.org/wiki/Attribute-value_system "Attribute-value system") would be a better approach. Rows and Columns could be a linked list, Cells could be in an indexed cache or hashtable. And the collections and individual objects would handle where they were in the domain of the spreadsheet.  
  
Guess I should get busy and prove my point.