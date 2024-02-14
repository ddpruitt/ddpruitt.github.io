---
title: 'Value Objects as Measurement Objects'
date: 2008-02-05T14:42:00.000-08:00
draft: false
xurlx: /2008/02/value-objects-as-measurement-objects.html
tags: 
- General
---

So I was reviewing PEAA looking to see if Fowler had come up with the idea of Measurment Objects, that horse I've been kicking around lately.  I found that he has a couple of close patterns:  Value Object and Money.  
  
Value Object:  A small simple object, like money or a date range, whose equality isn't based on identity.  
  
Money: Represents a monetary value.  
  
According to Fowler Value Objects are light weight and almost primative types.  They should also me immutable since they have no identity.  He also suggests that in .NET structs could be used as Value Objects.  
  
The money pattern looks like a special case of a Measurement Object with the UnitOfMeasure being set to currency (currency being USD or Euro).  
  
I see the usefulness of using structs but there is the boxing issue if we implement them with interfaces.  
  
I will have to ponder on this.