---
title: 'Parsing Delicious Export File'
date: 2010-12-25T04:24:00.000-08:00
draft: false
url: /2010/12/parsing-delicious-export-file.html
tags: 
- .Net
---

With all the brew ha ha going on about how Delicious is going to be dumped I made a backup of my [Delicious](http://www.delicious.com/ddpruitt) bookmarks. While I was at it I created a quick utility to parse the export file so I could play with the links and tags.  
  
Key to parsing the file was the [HTML Agility Pack](http://htmlagilitypack.codeplex.com/). The Bookmark class:  
  
```csharp  
public class Bookmark
{
    public string Title { get; set; }
    public string Href { get; set; }
    public DateTime AddDate { get; set; }
    public string AddDateEpoch { get; set; }
    public List<string> Tags { get; set; }
    public bool IsPrivate { get; set; }
    public Bookmark()
    {
        Tags = new List<string>();
    }

    public static Bookmark New(HtmlNode node)
    {
        if (node == null) throw new ArgumentNullException("node");

        var bookmark = new Bookmark
        {
            Title = node.InnerText ?? string.Empty,
            Href = node.Attributes["href"].Value ?? string.Empty,
            AddDate = FromUnixTime(Convert.ToDouble(node.Attributes["ADD_DATE"].Value ?? "0")),
            IsPrivate = (node.Attributes["ADD_DATE"].Value ?? "0").Equals("1")
        };

        bookmark.Tags.AddRange(GetTags(node.Attributes["tags"].Value ?? string.Empty));

        return bookmark;
    }

    protected static DateTime FromUnixTime(double unixTime)
    {
        DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
        return epoch.AddSeconds(unixTime);
    }

    protected static string[] GetTags(string tagList)
    {
        if (string.IsNullOrEmpty(tagList)) return new string[] { };

        return tagList.Trim().Split(',');
    }

}
```  
  
The tricky part was really just handling the Epoch time for the AddDate field.  
  
Using the class:  

```csharp  
var doc = new HtmlDocument();
doc.Load(@"Datadelicious.htm");

var bookmarks = doc.DocumentNode.SelectNodes("//a[@href]").Select(Bookmark.New);

var tags = bookmarks.SelectMany(b => b.Tags).Distinct().OrderBy(t => t);

var output = new StringBuilder();

foreach (var tag in tags)
{
    output.AppendLine(tag);
    string localTag = tag;
    var taggedBookmarks = bookmarks.Where(b => b.Tags.Contains(localTag)).OrderBy(b => b.AddDate);
    foreach (var taggedBookmark in taggedBookmarks)
    {
        output.AppendFormat("t{0}", taggedBookmark.Title).AppendLine();
    }
}

File.WriteAllText("TaggedBookmarks.txt", output.ToString());
Console.WriteLine(output.ToString()); 
  
```