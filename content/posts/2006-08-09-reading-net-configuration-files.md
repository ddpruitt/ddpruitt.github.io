---
title: 'Reading .NET Configuration Files'
date: 2006-08-09T08:20:00.000-07:00
draft: false
url: /2006/08/reading-net-configuration-files.html
tags: 
- .Net
---

I am currently working on a small app that will scan our environment and verify that all the configuration settings are correct.  One thing I found was that you can’t open just any .config file using System.Configuration, it has to be named the same as the calling assembly.

What a pain.