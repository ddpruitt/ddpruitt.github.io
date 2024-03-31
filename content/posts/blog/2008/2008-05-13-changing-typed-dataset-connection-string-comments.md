---
title: Changing Typed DataSet Connection String
date: 2008-05-13T13:26:00.000-07:00
draft: true
xurlx: /2008/05/changing-typed-dataset-connection-string.html
tags:
  - .Net
---

#### Still sounds like a workaround. Thats always was my...
[Beag]( "noreply@blogger.com") - <time datetime="2008-08-18T09:18:00.000-07:00">Aug 1, 2008</time>

Still sounds like a workaround. Thats always was my problem too. But... what do I do if I want, in DAL, at design time, work to my main project connection string?

#### It is a workaround and not one I really like. But...
[Darren Pruitt]( "noreply@blogger.com") - <time datetime="2008-08-18T09:33:00.000-07:00">Aug 1, 2008</time>

It is a workaround and not one I really like. But it worked so I just accepted it and moved onto the next issue.  
  
I would think that if you wanted to work in your main project at design time but point to your main DB, not your DAL DB, then something like:  
  
if (DesignMode)  
EquipmentConnectionSettings.SetEquipmentToConnectToMdb(mdbName);
<hr />
