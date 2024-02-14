---
title: 'Building a build server with CC.NET'
date: 2006-08-31T01:12:00.000-07:00
draft: false
xurlx: /2006/08/building-build-server-with-ccnet.html
tags: 
- .Net
---

So I have started implementing CruiseControl.Net at work and I have to say it is pretty fun.  I’ve built a bunch on Nant scripts to build our SOA middle tier, installed CC.Net and setup the first project.

  

What I am struggling with now is getting VSS to update the source files.  I think I am just missing something small but for some reason I can’t get the files to update.

  

Update
------

  

 I finally had to create NAnt scripts to update the actual source.  The VSS task in CC.Net only monitors the VSS DB for changes, it does not actually down load the latest source.  At least as far as I could tell it does not.

  

Next is to get the VSS labeled.  CC.Net will label the source but it is not descriptive enough so it will be a trip back to the NAnt scripts.