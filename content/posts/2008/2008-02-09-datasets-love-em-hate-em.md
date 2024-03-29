---
title: 'DataSets - Love em Hate em'
date: 2008-02-09T19:12:00.000-08:00
draft: false
xurlx: /2008/02/datasets-love-em-hate-em.html
tags: 
- .Net
- Rant
---

I have a love-hate relationship with DataSets.  On the one hand they are easy to use and contain lots of built in functionality.  On the other hand they are abused by almost everyone but at the same time severely under utilized, meaning that I rarely see anyone use most of the features available.  
  
I get the feeling that the majority of developers that use DataSets have blinders on:  DataSets are only useful for reading and writing data to the database.  End of story.  If you are an ASP.NET developer you absolutely refuse to use them and use DataReaders only.  And in the last three years I have not seen too many applications that use the typed DataSets that can be created in Visual Studio 2005.  
  
I don't like using DataSets because I prefer to use NHibernate or ActiveRecord to interact with databases.  But lately I have been having a change of heart as I have been delving more and more into using DataSets in the application I am currently working on.  There is a lot of functionality baked into DataSets that I have been overlooking and since I have been working with the strongly typed DataSets generated by VS 2005 I have to ask myself why I need to recreate it with POCO's.  
  
The way I am looking at DataSets now is not as an in-memory relational database but as a highly versatile data structure that handles a lot of things I need it to handle:  Relationships, constraints, databinding, serializable and data error information.  Sometimes I see it as being nothing more than a collection of Hashtables, each Hashtable being another collection of Hashtables.  And the fact that there is a designer to work with it makes it better.  
  
I worked on a home grown messaging system project, the message object orginated as an XSD that got converted to C# classes using the xsd.exe tool.  The tool didn't work to well (this was in .NET 1.1) so all the names and name spaces were so screwed that most of the classes had the name space in the class name.  Intellisense really sucked at times.  Me being who I am I started trying to figure out a simpler message object when it donned on me that a DataSet would have been a perfect fit for what they were doing.  A .NET 1.1 DataSet serialized to XML was ugly but in 2.0 it got a much needed face lift.   You should have seen them laugh at me when I suggested switching the message object out.  "You dummy, DataSets are for Databases!!  Ha Ha Ha, we're writing SOAP messages!"   
  
Yeah, I left that place.  
  
When it comes to data binding DataSets can't be beat.  WinForms have been especially built for binding to DataSets.  One thing I wished that would be easy to do is to bind one DataSet to another without using  form.  Currently I have a form that has two user controls, each with its own DataSet that require data in one form to update data in the other.  I am working on a way to databind the two DataSets together, currently the form is manually updating the data in the other DataSet as it changes.  I want to pull all the code out of the forms and create a Database Synch Service.  
  
A lot of the data validation I have seen in OPA's (other peoples apps) has been in the form.  Typcially if an error occurs a nice little pop-up shows some nasty message then either won't let the user save the data or reverts it back to a previous value.  DataSets have a way to set a Row and Column with a specific error message which can be used by the form to indicate there is an error.  
  
As for the actual validation, I want to create a [dictionary of methods](http://ebersys.blogspot.com/2006/09/dictionaries-of-methods-in-c.html "Dictionaries of methods in C#")  to validate the DataTables and DataColumns.  The validators would take the row, column and DataSet being validated and the validation errors will be entered using the rows SetColumnError method.  
  
Still though, DataSets are going to become mute when Linq comes out.  Maybe.