---
title: 'Json for jqGrid from ASP.Net MVC'
date: 2009-04-02T11:55:00.000-07:00
draft: false
url: /2009/04/json-for-jqgrid-from-aspnet-mvc.html
tags: 
- .Net
- asp.net-mvc
- Web Standard
---

[jqGrid](http://www.trirand.com/blog/ "jqGrid") takes a specific format for its json (taken from jqGrid documentation):  
  
```json
{  
    total: "xxx",
    page: "yyy", 
    records: "zzz",  
    rows : [  
        {id:"1", cell:["cell11", "cell12", "cell13"]},  
        {id:"2", cell:["cell21", "cell22", "cell23"]},  
        ...
    ]
}
```  
  
The tags mean the following:  
  
- total - Total number of Pages.  
- page - Current page Index.  
- records - Total number of records in the rows group.  
- rows - An array with the data plus an identifier.  
- id - The unique row identifier, needs to be an int from what I have found.  
- cell - An array of the data for the grid.  
  
The ASP.Net MVC framework has the JsonResult response type which we can use to populate the jqGrid. As an example I created a Person model and a method to return some data:  
  
``` csharp
public class Person  
{  
    public int ID { get; set; }  
    public string Name { get; set; }  
    public DateTime Birthday { get; set; }  
}  
  
public IEnumerable<Person> GetABunchOfPeople()  
{  
    yield return new Person {ID = 1, Name = "Darren", Birthday = new DateTime(1970, 9, 13)};  
    yield return new Person {ID = 2, Name = "Dawn", Birthday = new DateTime(1971, 6, 1)};  
    yield return new Person {ID = 3, Name = "Thomas", Birthday = new DateTime(1995, 10, 3)};  
    yield return new Person {ID = 4, Name = "Zoey", Birthday = new DateTime(1997, 8, 15)};  
}  
```  
  
Generating the JSON is as follows, this going into a PersonModel class:  
  
``` csharp
public JsonResult GetABunchOfPeopleAsJson()
{
    var rows = (GetABunchOfPeople()
    .Select(c => new
    {
        id = c.ID,
        cell = new[]
    {
        c.ID.ToString(),
        c.Name,
        c.Birthday.ToShortDateString()
    }
    })).ToArray();
    return new JsonResult
    {
        Data = new
        {
            page = 1,
            records = rows.Length,
            rows,
            total = 1
        }
    };
}
```  
  
  
The controller then would look like:  
``` csharp
public class PersonController : Controller  
{  
    public ActionResult Index()  
    {  
        return View();  
    }  

    public JsonResult GetAllPeople()  
    {  
        var model = new Models.PersonModel();  
        return model.GetABunchOfPeopleAsJson();  
    }  
} 
```  
The view doesn't need to inherit any model other than View page if all you want is for the jqGrid to show the data. In the view, Index.aspx in this case, you add table and div for the jqGrid:  

```html
<table id="dictionary" class="scroll" cellpadding="0" cellspacing="0"></table>  
<div id="pager" class="scroll" style="text-align: center;"></div>  
```

Configuring jqGrid is:  
```javascript  
$(document).ready(function() {  
  
    $("#dictionary").jqGrid({  
        caption: "Tank Dictionary",  
        pager: $("#pager"),  
        url: '<%= ResolveUrl("~/Person/GetAllPeople") %>',  
        editurl: '<%= ResolveUrl("~/Person/Edit") %>',  
        datatype: 'json',  
        myType: 'GET',  
        colNames: ['ID', 'Name', 'Birthday'],  
        colModel: [  
            { name: 'ID', index: 'ID', width: 150, resizable: true, editable: false },  
            { name: 'Name', index: 'Name', width: 200, resizable: true, editable: true },  
            { name: 'Birthday', index: 'Birthday', width: 300, resizable: true, editable: true }  
        ],  
        sortname: 'ID',  
        sortorder: 'desc',  
        viewrecords: true,  
        height: '100%',  
        imgpath: '<%= ResolveUrl("~/Scripts/jquery/jqGrid/themes/basic/images") %>'  
    });
}  
```  
The url tag is set to call the Person controller to get the JSON results. Note the datatype is json.  
  
The editurl tag will be talked about later.