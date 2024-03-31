---
title: 'Learning TypeMock.Net'
date: 2007-03-31T15:26:00.000-07:00
draft: false
xurlx: /2007/03/learning-typemocknet.html
tags: 
- .Net
---

We need to implement a better unit testing strategy at work but unfortunately we have code that is not written to be unit tested. Our unit test rely on the state of our database and this has caused so many issues that I'm hesitant to even run them.  
  
To fix the issue I have started working with [TypeMock.Net](http://www.typemock.com/ "TypeMock.Net") an AOP mocking tool.  TypeMock will intercept calls to objects that are called by the code you are testing without actually creating or executing the object.  I'm still fuzzy on the details but I plan on using this tool to help me test some pretty untestable code.