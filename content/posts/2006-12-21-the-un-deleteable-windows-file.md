---
title: 'The Un-Deleteable Windows File'
date: 2006-12-21T02:33:00.000-08:00
draft: false
url: /2006/12/the-un-deleteable-windows-file.html
tags: 
- General
---

I have several Open Source projects I am following and to facilitate downloading the code I created a simple NAnt script.  The script worked great until the other day one of the projects started failing to update.

Most of the projects I'm tracking are in Subversion.  All I'm doing is checking out the trunk then about once or twice a week doing an update.  So when the project started failing to update I figured it would be a simple matter of deleting the project folder and just checking it out again.

Wasn't going to happen.  One of the directories would no delete, no matter what I tried.  I tracked it down to a couple of file in hidden \_svn folders.  I could delete every other file in this folder except for these.  Nothing seemed to work.

I finally dropped to a command window and attempted to DEL the file, that is when I got the first real clue what the problem was:  the file name  was too long.  The root folder of all the projects I was updating was on my Desktop so the path was already super long: C:\\Documents and blah blah buried deep in the bowls of the hard drive long\\Desktop.

So I moved the root folder to my c:\\ and sure enough I was able to delete the folders.

Funny how that works.