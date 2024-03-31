---
title: 'My First WTF Experiance'
date: 2006-03-15T09:18:00.000-08:00
draft: false
xurlx: /2006/03/my-first-wtf-experiance.html
tags: 
- Rant
- General
---

OK, so I have seen bad code in the past.  Hell, I’ve written some pretty bad code in the past, present and will in the future.  But what I am looking at right now just blows my freaking mind.

A stored procedure that generates dynamic SQL to parse an XML document stored in a table which then somehow drives a cursor to fetch one row from something the dynamic sql calls that then drives another cursor that uses OPENXML to get data from another xml document.

I can’t figure it out!  Why?  Who was the clever SOB that thought this could even remotely be a good idea?

WTF?!!?