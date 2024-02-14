---
title: 'Asynchronous Programming in C#'
date: 2010-10-29T04:48:00.000-07:00
draft: false
url: /2010/10/asynchronous-programming-in-c.html
tags: 
- .Net
---

[Making Asynchronous Programming Easy](http://blogs.msdn.com/b/somasegar/archive/2010/10/28/making-asynchronous-programming-easy.aspx)  
  
[Asynchronous Programming in C#](http://blogs.msdn.com/b/ericlippert/archive/2010/10/29/asynchronous-programming-in-c-5-0-part-two-whence-await.aspx)  
  

```csharp
public async void AsyncIntroParallel()  
{  
    Task<string> page1 = new WebClient()
                .DownloadStringTaskAsync(new Uri("http://www.weather.gov"));  
    Task<string> page2 = new WebClient()
                .DownloadStringTaskAsync(new Uri("http://www.weather.gov/climate/"));  
    Task<string> page3 = new WebClient()
                .DownloadStringTaskAsync(new Uri("http://www.weather.gov/rss/"));  
    
    WriteLinePageTitle(await page1);  
    WriteLinePageTitle(await page2);  
    WriteLinePageTitle(await page3);  
}
```  
  
Async CTP has been released. Note: This is not automatically Multi-Threading! From the CTP:  

>   
> Merely sticking the "Async" modifier will not make a method run on the  
> background thread. That is correct and by design. If you want to run code on a  
> background thread, use TaskEx.Run(). Please read the overview for an explanation  
> of asynchrony.