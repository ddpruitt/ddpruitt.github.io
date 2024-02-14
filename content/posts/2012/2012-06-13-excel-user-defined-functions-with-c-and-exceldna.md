---
title: 'Excel User Defined Functions with C# and ExcelDna'
date: 2012-06-13T05:35:00.002-07:00
draft: false
xurlx: /2012/06/excel-user-defined-functions-with-c-and_58.html
tags: 
- .Net
- MS Excel
---

Adding user defined functions to Excel can be laborious, either you use VBA or COM. Why can't Microsoft make it easier to use Visual Studio and .NET within Excel?  
  
Enter [ExcelDna](http://exceldna.codeplex.com/ "ExcelDna on CodePlex").  
  
  
  
This example creates a static function in C# using Visual Studio. There is documentation that already describes this, what I am adding is:  

  
*   Use Visual Studio to create project  
    
*   Post Build Events to help create the final XLL file.  
    
  
**NOTE:** _I hate Post Build Events._ The ONLY exception is in this case where I am modifying the output of the final build. Post Build Events are Evil especially when used in a large development team. A better solutions is to create actual Build Scripts that do what I am about to do. You have been warned.  
  
First, create a new Class Library project. Use NuGet to add a reference to the Excel-DNA package. NuGet will also add ExcelDna.dna and ExcelDna.xll to your project. Rename them both to the name that you want your final output xll to be. In my case I renamed them to Scratch-ExcelDna.dna and Scratch-ExcelDna.xll. Also for both files change to properties for **Build Action** to "Content" and **Copy to Output Directory** to "Copy Always".  
  
Within the packages folder created by NuGet is the ExcelDnaPack utility, in the tools folder. This will package your project into one xll file. For it to work you need to update the .dna file:  
  
```
<DnaLibrary RuntimeVersion="v4.0">
      <ExternalLibrary Path="DC.Scratch.ExcelDna.dll" Pack="true" />
      </DnaLibrary>
      
```  
  
Note that I am using .Net 4.  
  
To get the pack utility to work, add a Pre-build event to delete the existing xll:  
  
del "$(TargetDir)Scratch-ExcelDna-packed.xll"  
  
Then add a Post-build event to recreate it:  
  
"$(SolutionDir)packagesExcel-DNA.0.29toolsExcelDnaPack.exe" "$(TargetDir)Scratch-ExcelDna.dna"  
  
While you are in the Project Properties messing with the evil build events set the Debug options so you can test your code. Set the External Program to you MS Excel and add a Command Line argument with a path to your final xll file.  
  
Now, add a class TestFunctions:  

```csharp  
using ExcelDna.Integration;  
  
namespace DC.Scratch.ExcelDna  
{  
      public class TestFunctions  
      {  
            [ExcelFunction(Description = "Product of two numbers", 
                           Category = "DNA Test")]  
            public static double TheProductOf(double x, double y)  
            {  
                  return x*y;  
            }  
      }  
}  
```  
  
Hit F5 and see if Excel Starts. If it does, add some numbers to the Excel spreadsheet and see if the TheProduct() function works.  
  
Download sample project here: [DC.Scratch.ExcelDna](https://dl.dropboxusercontent.com/u/480457/techshorts/2012/06/DC.Scratch.ExcelDna.zip)  
  
Microsoft should really look at buying Excel-DNA and incorporating it into Visual Studio. You Guys Listening!?!