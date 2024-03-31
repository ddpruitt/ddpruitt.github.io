---
title: 'Coding Horror: Properties vs. Public Variables'
date: 2006-10-31T05:36:00.000-08:00
draft: false
xurlx: /2006/10/coding-horror-properties-vs-public.html
tags: 
- General
---

> Why waste everyone's time with a bunch of meaningless just-in-case wrapper code? Start with the simplest thing that works-- a public variable. You can always refactor this later into a property if it turns out additional work needs to be done when the name value is set. If you truly need a property, then use a property. Otherwise, KISS! Update: As many commenters have pointed out, there are valid reasons to make a trivial property, exactly as depicted above:  
>   
> \* Reflection works differently on variables vs. properties, so if you rely on reflection, it's easier to use all properties.  
>   
> \* You can't databind against a variable.  
>   
> \* Changing a variable to a property is a breaking change.

  
Source: [Coding Horror: Properties vs. Public Variables](http://www.codinghorror.com/blog/archives/000654.html)  
  
So this answers the question on why you use properties in .NET and not just public variables.  
  
I've always used properties but on occasion of used public variables. Small object, needed it quick so didn't bother with the getter or setters.  
  
I work with a Java developer who abhors getters and setters, says it is Microsoft's way of disguising method calls. I disagree since I don't think the overhead involved is that great.  
  
As far as creating the getters and setters, hey, that is what code snippets is for.