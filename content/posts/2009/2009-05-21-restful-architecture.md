---
title: 'RESTful Architecture'
date: 2009-05-21T11:48:00.000-07:00
draft: false
url: /2009/05/restful-architecture.html
tags: 
- .Net
- asp.net-mvc
---

Found the following in the article [ASP.NET MVC: Using RESTful Architecture](http://blog.wekeroad.com/2007/12/06/aspnet-mvc-using-restful-architecture/) and thought it worth noting. It describes what the MVC command actions should be, no more than this is needed:  

> The endpoint needs to be something meaningful, and Rails uses a nice convention that divides the endpoints into 7 main bits:  
>   
> *   Index - the main “landing” page. This is also the default endpoint.
> *   List - a list of whatever “thing” you’re showing them - like a list of Products.
> *   Show - a particular item of whatever “thing” you’re showing them (like a Product)
> *   Edit - an edit page for the “thing”
> *   New - a create page for the “thing”
> *   Create - creates a new “thing” (and saves it if you’re using a DB)
> *   Update - updates the “thing”
> *   Delete - deletes the “thing”
>   
> Normally the last 3 are “action only ” and don’t have a view associated with them. So if you “create” a Product (from the New view, using Create as the action on the form), you’d just redirect then to the List or Edit views. Likewise if you Update a Product from the Edit page (using Update as the action on the form) you might want to go back to the Edit view and show a status update.
  
I don't agree that this should be the ONLY command results you can have but I think it is a good guideline to follow. Now I have to go update my MVC commands.