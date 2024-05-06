---
title: Evernote to Obsidian
date: 2024-05-05T08:49:00
draft: false
tags:
  - obsidian
  - evernote
---
## The Problem
I've used [Evernote](https://evernote.com/) since at least 2009 and I loved it.  I didn't use it for note taking, it was primarily my brain and web clipping dump.  The Evernote web clipper is the best out there and still is today.

But times change and Evernote's cost increased to the point where it didn't make economic sense to keep it.  It is still a good product but as I started looking for a replacement I realized that the main thing I used it for was the web clipper.

After reviewing the alternatives I settled on using [Obsidian](https://obsidian.md/) to manage my notes and store the Obsidian vault in git.  The advantages are 
- Not locked into a proprietary note format
- Cloud storage
- History of changes built in

Next I wanted to convert all of my existing notes in Evernote to Markdown files so I wouldn't loose all my note history.  Granted, I am a web clipping hoarder and I should really clean up the notes, but I can do that later.
## The Conversion
### Exporting from Evernote
Used to be I could run the Evernote application locally and have it export a single XML file for all of my notes.  But then they went and changed it.

Now (as of 2024) I had to go to each Notebook in the desktop application and export it individually to the \*.enex file format.   This took me a long time since I had a lot of Notebooks and Notebook Stacks.  I wound up
- Creating a root Backup folder.
- For each Stack create a Stack Folder.
- Then for each Notebook in the Stack export it to the Notebook.enex file.
- If a Notebook was not in a Stack then export it to the root Backup folder.

This took me a while to do but once it was done I de-activated my Evernote account.  That was a sad day for me.

### Direct Import into Obsidian
Obsidian has the Importer plug-in to directly import ENEX files.  But it is one ENEX at a time so it will take a long time to import all of the files.  Also I found that if the ENEX is large enough the import can fail.

### evernote2md.exe
[evernote2md](https://github.com/wormi4ok/evernote2md)

> Evernote2md is a CLI tool to convert Evernote notes exported in \*.enex format to a directory with markdown files.

Instead of manually running the CLI tool for each ENEX file a script can be written to scan for all of the ENEX files and convert them.  For example:

```powershell
# Define the root directory to scan
$rootDirectory = "Path\to\Evernote\Backup"

# Define the output directory
$outputDirectory = "Path\to\Evernote\output"

# Get all .enex files in the root directory and its subdirectories
$enexFiles = Get-ChildItem -Path $rootDirectory -Recurse -Include "*.enex"

# Loop through each .enex file
foreach ($file in $enexFiles) {
    # Define the output file path
    $outputFilePath = Join-Path -Path $outputDirectory -ChildPath ($file.BaseName)

    # Execute the evernote2md.exe command
    & "Path\to\evernote2md.exe" $file.FullName $outputFilePath
}
```
## Web Clipping
### Markdownload
[markdownload](https://github.com/deathau/markdownload)

> This is an extension to clip websites and download them into a readable markdown file. Please keep in mind that it is not guaranteed to work on all websites.

Markdownload is not a perfect replacement for the Evernote web clipper but it will save web sites in Markdown, which is what I want.