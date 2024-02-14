---
title: 'FxCop and NAnt'
date: 2006-12-06T08:41:00.000-08:00
draft: false
url: /2006/12/fxcop-and-nant.html
tags: 
- General
---

I am working on automating our build process and one of the things I am trying to do now is to add FxCop to the mix.  There are a couple of ways to run the process, one using an exec task to call the executable directly, the other is through a Contrib task <fxcop>.

I tried the first method with no success.  For some reason I couldn't get the output file name to be recognized by the executable.  I eventually had to switch to the <fxcop> task but I had to set a system envitonment variable to the executable first.  It eventually looked like this:

```xml
<target
     name="analyze.fxcop" 
     description="Runs FxCop on build output">  
     <setenv name="PATH" value="${tools.dir}\fxcop;%PATH%" />  
     <mkdir dir="${build.dir}\fxcop" />  
     <fxcop
          directOutputToConsole="false" 
          analysisReportFilename="${build.dir}\fxcop\fxcop.xml"
          failonerror="false">  
          <targets>  
               <include name="${build.dir}\release\bin\*.dll" />  
          </targets>  
     <dependencyDirectories refid="referenceComponents"/>  
</fxcop>  
  
</target>
```  

A lot of work was involved just to find this out.  Oh well, I got it working so now I share my results.