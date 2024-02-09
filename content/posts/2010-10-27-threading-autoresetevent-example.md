---
title: 'Threading AutoResetEvent Example'
date: 2010-10-27T20:25:00.000-07:00
draft: false
url: /2010/10/threading-autoresetevent-example.html
tags: 
- .Net
---

```csharp
public class Program  
{  
    private static System.Threading.AutoResetEvent stopFlag = new System.Threading.AutoResetEvent(false);  

    public static void Main()  
    {  
        ServiceHost svh = new ServiceHost(typeof(ServiceImplementation));  
        svh.AddServiceEndpoint(  
            typeof(WCFSimple.Contract.IService),  
            new NetTcpBinding(),  
            "net.tcp://localhost:8000");  

        svh.Open();  

        Console.WriteLine("SERVER - Running...");  

        stopFlag.WaitOne();  

        Console.WriteLine("SERVER - Shutting down...");  

        svh.Close();  

        Console.WriteLine("SERVER - Shut down!");  
    }  
    
    public static void Stop()  
    {  
        stopFlag.Set();  
    }  
}
```