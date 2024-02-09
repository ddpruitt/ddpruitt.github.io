---
title: 'EntLib Data Access Block and Excel'
date: 2007-10-10T04:39:00.001-07:00
draft: false
url: /2007/10/entlib-data-access-block-and-excel.html
tags: 
- General
---

I need to be able to open an Excel workbook and grab the data from one of the worksheets. I don't want to use any Office InterOperability (as that sucks) and I know I can do it using ADO.Net. But since 1.) I hate writing all of that DataAdapter / Connection code because 2.) I am lazy I decided to see how hard it would be to use the EntLib DAB.  
  
Turned out it was pretty easy.  
  
Create your configuration as such:  
  
Then call your code as such:  
  
```csharp  
public static DataSet GetWorksheet()  
{  
    string sql = "SELECT * FROM [Sheet1$]";  
    string connectionName = "Book1";  
    
    return DatabaseFactory
        .CreateDatabase(connectionName)  
        .ExecuteDataSet(CommandType.Text, sql);  
}  
```

And that is that. One DataSet ready to go.