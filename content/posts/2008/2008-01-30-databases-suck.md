---
title: 'Databases Suck'
date: 2008-01-30T14:55:00.000-08:00
draft: false
url: /2008/01/databases-suck.html
tags: 
- General
---

I am sick of working with databases, they all suck.  I can generalize this to any type of data store, they all suck.  
  
OK, they don't really suck but I sure have seen a lot of database designs that do.  I am tired of working with applications that have lots and lots of large tables, tables with lots and lots of fields.  Of course all the fields but the primary key are nullable because you might not always need that extra address field but it is there, just in case.  
  
And don't get me started on compound primary keys.  If I see tables with compound primary keys then I am trying to get off of that project.  
  
What if we have to change the database, add a field, remove a field or just plain rename a field?  Well that just broke the application, not just the code you own but the code in a module you never heard of that the reporting team is using.  Why can we not update schema's without worrying about causing the build to fail?  
  
And what about all the boiler plate code needed to communicate with your database?  We have to invest in code generators that get it about 80% right.  Then the databinding to the forms, after all a database does no good unless you can get data into it.  
  
So how do we solve this problem?  Get ride of the database?  Use only CSV files?  Go back to pencils and paper?  Those are options but not necessarily good ones.  We have the technology so why not use it, only use it in a different way.  
  
What are we trying to do with the database any way?  Store values so we can retrieve them later.  
  
What about history?  How do we know when someone changed a value?  And what was the value before?  
  
What is a value?  If we don't know what it is we are storing then how are we going to know how to store it?  
  
Based on the majority of the applications I have worked on values tend to be descriptions or measurements.  Descriptions are remarks, model numbers, serial numbers or some other form of text that has meaning to the object.  Measurements are attributes of an object that magnitude relative to some unit of measurement system.  [Wikipeda](http://en.wikipedia.org/wiki/Measurement) has a better description of it than I do so check it out then come back.  
  
Note that this is not a new idea, I read about measurement values somewhere but I can't for the life of me find who actually came up with it orginally.  And if you really want to get technical about measurement values there are scalers (magnitude only) and vectors (magnitude and direction).  
  
So how do we handle this in databases?  First we create a measurement table like such:  

![Measurement Table](http://writer.zoho.com:80/ImageDisplay.im?name=344506000000007001/1201752124371_MeasurementTable.png&accId=344506000000002007)  

This table is pretty basic:  You have your primary key Id, the magnitude of the measurement is the value and the system used is determined by the UnitOfMeasurement.  The CreatedBy and CreatedOn is for audit and historical purposes.  To maintain a history in the database we will not allow deletes from this table, only inserts.  This allows us to see all the changes made to this value when they were made and by who.  The value that was added last is the current value.  
  
Now that we have the measurement what exactly are we measuring?  We need some properties of some kind.  So we need a Property table and a link to the Measurement table.  

![Property Measurement](http://writer.zoho.com:80/ImageDisplay.im?name=344506000000007001/1201752957798_PropertyMeasurement.png&accId=344506000000002007)  

We have the Id again which becomes the foreign key in the Measurement table.  The name is what the property is called and the TypeOfMeasurement is the attribute being measure.  This could be length, mass, weight, currency, velocity, acceleration, etc.  Note that it is the UnitOfMeasurement in the Measurement table that dictates if we use feet, meters, kilograms, pounds, US Dollars, Euros, meters per second, etc.  
  
What do these properties apply to though?  We need an object that has properties:  

![Object Properties](http://writer.zoho.com:80/ImageDisplay.im?name=344506000000007001/1201753728226_objectProperties.png&accId=344506000000002007)  

  
Now this is just getting crazy.  Our object can have many properties but also our properties can belong to many objects.  We will have to work that little kink out but for now I think I will stop playing with pictures.  
  
What is it I really am trying to do?  I want to reduce the pain of making changes to the database for an application.  What does that mean?  
  
Let us say for example we create an app that needs to track squares.  Nothing fancy, just a square.  Our first instinct is to create a table such as:  
![Square](http://writer.zoho.com:80/ImageDisplay.im?name=344506000000007001/1201754189141_square.png&accId=344506000000002007)  
  
After building many forms and reports off of our square table management comes along and says we need to track cubes!  Those eF'ers!  You have to go back, change the name of the table, wait, no just leave the name the same, I'll remember what it really should be.  Add the new column Height, modify all the data access code, forms and reports to use the new value and life goes on.  
  
Till six months later when management comes back and says that some federal requirements mandate that all changes to the cubes be tracked for three years.  So you look for your cube table which doesn't exist.  There is this square table with all the right fields, but squares don't have height, they are flat!  Screw it just add a history table, copy the rows as they are changed, create some triggers, job done.  
  
Except management finds out that Canada has been entering values in metric but in the states it is in feet.  Plus Canada has been updating some of the US'es cubes.  Now is the time to get the resume ready.  
  
Databases suck.  Working with databases is a pain but it really shouldn't be.  This extends beyond databases, it applies to objects as well.  How to stop the pain?  Stop adding values directly to the table.  Granted this is not a 100% solution, I mean really if I had a square object I wouldn't worry a lot about keeping track of two fields.  
  
But if I had some complicated object that had many properties, and I was still in the development cycle where all of the properties still had not been identified, I think I would take the Measurement table approach.  

  
Technorati Tags     [database](http://technorati.com/tag/database)