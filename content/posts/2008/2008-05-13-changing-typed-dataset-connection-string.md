---
title: 'Changing Typed DataSet Connection String'
date: 2008-05-13T13:26:00.000-07:00
draft: false
url: /2008/05/changing-typed-dataset-connection-string.html
tags: 
- .Net
---

I was working on a WinForm app that connected to a MS Access database. Yeah, Access sucks but I didn't have a choice in the matter.  
  
The app itself is used to import a bunch of CSV files into the Access database. It is more of a utility program and it has going through several variations, from being a simple hand driven command line tool to being GUI driven.  
  
The command line version was all hand controlled. I had to go in and update configuration files to point to the CSV files and the MDB database. That got old fast so I decided to make a GUI version that would allow me to pick the MDB file and each of the CSV files to import. Picking and using the CSV files was easy, it was changing the connection string for the MDB that proved to be the hardest.  
  
I am using strongly typed datasets in VS2005. If you have ever worked with them you find out soon that the connection string gets saved with the project in the app.config file, even if it is a separate DAL dll project. My guess is that Microsoft assumed that if you ever had a connection to one database then your strongly typed dataset would not have to change to another database, but if you did you could always just update the configuration file.  
  
As my drill instructor was always fond of saying: "Wrong f\*\*\*ing answer!"  
  
I wanted, in the case, to use a strongly typed dataset. I also wanted to use different Access files, and I wanted to be able to select which Access file I used while the application was running. Why is that so hard?  
  
I searched the web and found several not so useful suggestions. It appears that there are two camps of people for this issue: Those that understood what I wanted to do and were trying to do it also, and those that didn't understand what the problem was.  
  
Those in the latter camp always resorted to the same suggestion: Just update the configuration files to point to the new database. This doesn't work in this case because I would then have to restart the app in order for the new setting to be picked up.  
  
The other popular option was to completely re-write the Settings.Designer.cs file so that when the DataTables called to get the connection string it would call a method you created so you could pass anything into it you wanted. The problem with this approach is that if you changed any of the other settings then your code would get over-written by Visual Studio.  
  
The least popular option was to go to each DataTable and create partial classes that override the InitConnection() method. This royally sucks if you have lots of DataTables.  
  
In the Settings class, all Connection String types are application level and cannot be made into User Settings. This leads to the other issue, namely all Connection String settings are read-only. But this is true only for saving the connection string, which is not something I needed to do. I just need to be able to change it.  
  
Also the Settings class is internal and sealed, which means it cannot be accessed from outside of the current project. So my GUI project cannot directly access my DAL project and update the Settings value. To get around that limitation I just created a proxy class. The resulting class is:  
  
  

```csharp
using DC.Catalog.DAL.Properties;
namespace DC.Catalog.DAL
{
      public static class EquipmentConnectionSettings
      {
            public static void SetEquipmentToConnectToMdb(string MdbFilename)
            {
                  // This only overrides the in-memory copy of the setting. 
                  // It is not permanent.
                  Settings.Default["EquipmentConnectionString"] = string.Format(
                        Settings.Default.EquipmentConnectionStringTemplate
                        , MdbFilename);
            }
      }
}
```

Notice that to update the setting value I had to use:  

```csharp
Settings.Default["EquipmentConnectionString"]
```

  
Since the setting Settings.Default.EquipmentConnectionString is readonly I had to use the other way to access the value. The EquipmentConnectionStringTemplate is simple:  

```
Provider=Microsoft.Jet.OLEDB.4.0;Data Source={0}  
```

Every time I change the Access file I am pointing at I call the SetEquipmentToConnectToMdb() method, magic happens in the Typed DataSet and it all just works.