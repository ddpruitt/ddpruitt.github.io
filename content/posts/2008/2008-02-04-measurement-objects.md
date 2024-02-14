---
title: 'Measurement Objects'
date: 2008-02-04T14:14:00.000-08:00
draft: false
url: /2008/02/measurement-objects.html
tags: 
- General
---

In my previous post I described setting up a database to track measurements.  I ran into problems quickly with the data model and I could see that it was going to become a convoluted mess pretty quick.  So I want to take a step back and look at it from an application point of view.  
  
What I want is a way to track an objects measurements.  I need a system that is easy to understand, any one looking at an object should readily understand what each measurement property is and what units it is in.  The system should accommodate almost any existing system of measurement but at the same time it needs to be simple.  Brain dead simple.  
  
![Measurement Object Diagram](http://writer.zoho.com:80/ImageDisplay.im?name=344506000000008001/1202182817737_MeasurementObject.png&accId=344506000000002007)  
What I am thinking is to start out with a abstract base class for our Measurement Object.  It has an ID that can be an int, string or Guid, take your pick.  The Text property is the string representation of the Value, Value being in an abstract class that extends MeasurementBase.  
  
I think that it is important to seperate the actual Value into another class of Type T.  This allows us to create collections of MeasurementBase objects and add MeasurementBase<T> objects to it.  I am still struggling with this idea as it totally blows apart expectations on what a Value is (is it an int?  a DateTime?), but this is the reason I stuck the Text property on the MeasrumentBase.  The Text property takes care of the conversion of the string representation to the actual Value.  
  
Already this is getting complicated.  What is it I'm trying to do here?  
  
My ultimate goal is to have the means to convert a measurment from one system to another.  The objects themselves should take care of the conversion.  The way I have seen it in the past is to have conversion code scattered throughout the entire application layer, including the UI and the data access layer.  
  
This is one of those ideas that is on the tip of my neurons, I know it has been solved before but I haven't found it.  And every time I start working on it I naturally want to drop into the Database thinking frame of mind.  
  
I need to review some of the Enterprise Patterns, see if it already exits.