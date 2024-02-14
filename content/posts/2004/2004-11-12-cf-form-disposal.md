---
title: 'CF Form Disposal'
date: 2004-11-12T04:40:00.000-08:00
draft: false
xurlx: /2004/11/cf-form-disposal.html
tags: 
- .Net
---

The December 2004 issue of **MSDN** contains an article on developing CF apps. Part of the article advocates the disposing of forms as soon as you are done using them in order to conserve memory. I disagree with this.  
  
I have found that caching forms improves the performance of a CF application. It takes a long time to load a form in CF so why should I make the user wait every time he has to reopen the same form? If the speed of creating forms was not an issue then I would be OK with disposing of them when done.  
  
There is a Form Stack code block floating around the web that I have been using successfully to cache forms. This works up to a point but it has limitations as well. I am working on a hashtable cache for forms and will extend it to include panels. I think that performance would be even more improved if instead of caching entire forms we cache the panels.