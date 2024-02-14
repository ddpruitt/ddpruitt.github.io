---
title: 'The Virtual Domain'
date: 2006-12-07T02:09:00.000-08:00
draft: false
xurlx: /2006/12/the-virtual-domain.html
tags: 
- General
---

I've been playing around with VMWare and vitalization for a while now and I have to say I really like it.  I have a virtual Ubuntu image that I am using to learn Ruby on Rails with.  I installed VMWare's server on my file server so I can run VM's from any PC in my house.

So last night I was working on my gateway PC when the thought hit me:  Why not create my own virtual domain?  I could create an image and install IPCop on it, add three virtual Ethernet cards to the VM and bridge the fourth to a real NIC and viola - Instant Domain.  Just add more servers.

From there I could add servers to the Green, Orange and Blue Virtual zones and just play around with configurations.  I could create a mini-web farm, play around with MySQL replication, a virtual cluster, anything.

Granted I would bump into a Hardware wall pretty quick, I dumped all my old PC's so now I don't have that many to play with.  But I bet I could still but together something pretty cool.