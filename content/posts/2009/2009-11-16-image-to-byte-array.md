---
title: 'Image to Byte Array'
date: 2009-11-16T08:09:00.000-08:00
draft: false
url: /2009/11/image-to-byte-array.html
tags: 
- .Net
---

In [Some ways to convert a byte array to Bitmap](http://team.sfi.vn/post/Some-ways-to-convert-a-byte-array-to-Bitmap.aspx "Some ways to convert a byte array to Bitmap") we see how to convert a Bitmap to and from a ByteArray. As an alternative I use the following extension method to convert an image to a ByteArray:  
  
```csharp
using System.Drawing;  
namespace DC.Core.Extension  
{  
    public static class ImageExtension  
    {  
        private static ImageConverter _converter;  
        
        public static byte[] ToByteArray(this Image image)  
        {  
            if (_converter==null) _converter = new ImageConverter();  
            return (byte[])_converter.ConvertTo(image, typeof(byte[]));  
        }  
    }  
}  
```