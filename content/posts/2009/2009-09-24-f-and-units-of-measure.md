---
title: 'F# and Units of Measure'
date: 2009-09-24T07:06:00.000-07:00
draft: false
xurlx: /2009/09/f-and-units-of-measure.html
tags: 
- .Net
---

On Channel 9 they have an [interview](http://channel9.msdn.com/posts/Charles/Andrew-Kennedy-F-Units-of-Measure/) with [Andrew Kennedy](http://blogs.msdn.com/andrewkennedy/) about the new Unit of Measure feature in F#.  
  
This is a really cool feature and I have plans to use it, however there is one problem I have with it: The UOM information is baked into the compiler and can't be persisted.  
  
An example that Andrew uses it the NASA's Mars Climate Orbiter that crashed due to an issue with Units of Measure (long story short the program thought it was using Newtons when it was actually using Pounds Force). This is a good example of how when you are doing calculations it would be nice if something other than the brain of the programmer was checking the units. But there is another time that UOM is important: When inputting the data.  
  
I recently worked on an application where the units for a given value could be in English or SI (metric) units and we had an elaborate system to deal with them. The end user could even come up with their own unit system if they wanted to and the application had to be able to handle that.  
  
Internally and for storage we degreed that all units would be in metric, no If's And's or But's about it. It was only when we displayed the values to the end user did we convert it to their chosen unit system. And for importing data we required that the imported values have their UOM indicated.  
  
The F# UOM feature is a truly remarkable step in solving one of my pet issues. The next step will have to include a way to persist the UOM information with the data to any data store because without this there is no way to guarantee that the data values coming in are really what we say they are.