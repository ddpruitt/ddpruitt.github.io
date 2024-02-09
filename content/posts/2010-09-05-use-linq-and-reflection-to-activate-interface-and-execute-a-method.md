---
title: 'Use Linq and Reflection to Activate Interface and Execute a Method.'
date: 2010-09-05T07:58:00.000-07:00
draft: false
url: /2010/09/use-linq-and-reflection-to-activate.html
tags: 
- .Net
---

I created a simple ICommand interface similar to the one in System.Windows.Input:  

```csharp
interface ICommand  
{  
    string Description { get; }  
    bool CanExecute(object parameter);  
    void Execute();  
}
```

In my project I created a bunch of classes that implement my ICommand interface and I wanted to find and execute them all.  I came up with two ways to do this, using a List<ICommand> and reflection.  

## List<ICommand>
  
This is very direct and easy but as I added new commands the list began to grow. Also I had to remember to add my commands to the list as I created them.  

```csharp
string separator = new string('=', 100);  
var commandsToExec = new List<ICommand>  
{  
    new Commands.TestEntityConnectionStringBuilderCommand(),  
    new Commands.BenchmarkingExampleCommand(),  
    new Commands.FastTokenizerBenchmarkCommand(),  
    new Commands.LinqToXmlTest01()  
};  

foreach (var cmd in commandsToExec)  
    if (cmd.CanExecute(null))  
    {  
        Console.WriteLine("{0}{1}{2}{1}{0}{1}", separator, Environment.NewLine, 
                            cmd.Description);  
        cmd.Execute();  
        Console.WriteLine("{1}{0}{1}{1}", separator, Environment.NewLine);  
    }  
```

## Reflection
  
Using linq I find all the types in my Assembly that implements the ICommand interface.  I then loop through the results, create an instance of the object and cast it as a ICommand and then call the execute method.  

```csharp 
var commandClasses = from type in Assembly.GetExecutingAssembly().GetTypes()  
    where type.GetInterfaces().Contains(typeof(ICommand))  
    select type;  

foreach (var commnadType in commandClasses)  
{  
    var cmd = Activator.CreateInstance(commnadType) as ICommand;  
    if (cmd.CanExecute(null))  
    {  
        Console.WriteLine("{0}{1}{2}{1}{0}{1}", separator, Environment.NewLine, 
                        cmd.Description);  
        cmd.Execute();  
        Console.WriteLine("{1}{0}{1}{1}", separator, Environment.NewLine);  
    }  
}  
```