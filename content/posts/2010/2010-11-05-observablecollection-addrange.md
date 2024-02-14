---
title: 'ObservableCollection AddRange'
date: 2010-11-05T07:15:00.000-07:00
draft: false
url: /2010/11/observablecollection-addrange.html
tags: 
- .Net
---

[Damon Payne](http://www.damonpayne.com) has a post [AddRange for ObservableCollection in Silverlight 3](http://www.damonpayne.com/2010/03/04/AddRangeForObservableCollectionInSilverlight3.aspx). It is pretty short and sweet way to improve performance when batch adding data to an ObservableCollection.  
  
He created a SmartCollection:  
  
```csharp  
public class SmartCollection<T> : ObservableCollection<T>
{
    public SmartCollection()
    {
        _suspendCollectionChangeNotification = false;
    }

    bool _suspendCollectionChangeNotification;

    protected override void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
    {
        if (!_suspendCollectionChangeNotification)
        {
            base.OnCollectionChanged(e);
        }
    }

    public void SuspendCollectionChangeNotification()
    {
        _suspendCollectionChangeNotification = true;
    }

    public void ResumeCollectionChangeNotification()
    {
        _suspendCollectionChangeNotification = false;
    }

    public void AddRange(IEnumerable<T> items)
    {
        this.SuspendCollectionChangeNotification();
        try
        {
            foreach (var i in items)
                base.InsertItem(Count, i);
        }
        finally
        {
            this.ResumeCollectionChangeNotification();

            var arg = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset);
            this.OnCollectionChanged(arg);
        }
    }
}  
```  
  
I created a scratch / test app to see if it works:  
  
```xml
<UserControl  
x:Class="SilverlightApplication10.MainPage"  
xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"  
xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"  
xmlns:d="http://schemas.microsoft.com/expression/blend/2008"  
xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"  
xmlns:my="clr-namespace:System.Windows.Controls;assembly=System.Windows.Controls.Data"  
mc:Ignorable="d"  
d:DesignHeight="300" d:DesignWidth="400">  
  
<Grid x:Name="LayoutRoot" Background="White">  
<StackPanel Orientation="Horizontal">  
<StackPanel Margin="5">  
<StackPanel Orientation="Horizontal" Margin="5">  
<TextBlock x:Name="SlowText" Height="50" Width="100"/>  
<Button x:Name="SlowButton" Content="Slow" Click="SlowButtonClick"/>  
</StackPanel>  
<my:DataGrid x:Name="SlowDataView"  
Height="300"  
AutoGenerateColumns="True"  
RowBackground="Cornsilk"  
AlternatingRowBackground="LightGray"/>  
</StackPanel>  
  
<StackPanel Margin="5">  
<StackPanel Orientation="Horizontal" Margin="5">  
<TextBlock x:Name="FastText" Height="50" Width="100"/>  
<Button x:Name="FastButton" Content="Fast" Click="FastButtonClick"/>  
</StackPanel>  
<my:DataGrid x:Name="FastDataView"  
Height="300"  
AutoGenerateColumns="True"  
RowBackground="Cornsilk"  
AlternatingRowBackground="LightGray"/>  
</StackPanel>  
</StackPanel>  
</Grid>  
</UserControl>  
```  
  
```csharp  
public partial class MainPage : UserControl
{
    public MainPage()
    {
        InitializeComponent();
        _slowCollection = new ObservableCollection<MyClass>();
        SlowDataView.ItemsSource = _slowCollection;

        _fastCollection = new SmartCollection<MyClass>();
        FastDataView.ItemsSource = _fastCollection;
    }

    #region Slow  

    private readonly ObservableCollection<MyClass> _slowCollection;
    private void SlowButtonClick(object sender, RoutedEventArgs e)
    {
        var timeSpan = Time(() => { for (int i = 0; i < 25000; i++) _slowCollection.Add(MyClass.BuildRandom()); });
        Dispatcher.BeginInvoke(() => this.SlowText.Text = timeSpan.ToString());
    }

    #endregion

    #region Fast  

    private readonly SmartCollection<MyClass> _fastCollection;
    private void FastButtonClick(object sender, RoutedEventArgs e)
    {

        var timeSpan = Time(() =>
        {
            var list = new List<MyClass>();
            for (int i = 0; i < 25000; i++) list.Add(MyClass.BuildRandom());
            _fastCollection.AddRange(list);
        });


        Dispatcher.BeginInvoke(() => this.FastText.Text = timeSpan.ToString());
    }

    #endregion

    private static TimeSpan Time(Action a)
    {
        var start = DateTime.Now;
        a();
        var end = DateTime.Now;
        return end - start;
    }
}
```  
  
```csharp  
internal class MyClass : INotifyPropertyChanged
{
    private static Random _random;

    #region Id  

    private string _id;

    public string Id
    {
        get { return _id; }
        set
        {
            if (value != _id)
            {
                _id = value;
                NotifyPropertyChanged("Id");
            }
        }
    }

    #endregion

    #region SomeRandomNumber  

    private int _someRandomNumber;

    public int SomeRandomNumber
    {
        get { return _someRandomNumber; }
        set
        {
            if (value != _someRandomNumber)
            {
                _someRandomNumber = value;
                NotifyPropertyChanged("SomeRandomNumber");
            }
        }
    }

    #endregion

    #region INotifyPropertyChanged  

    public event PropertyChangedEventHandler PropertyChanged;

    public void NotifyPropertyChanged(string propertyName)
    {
        var propertyChanged = PropertyChanged;
        if (propertyChanged != null)
            propertyChanged(this, new PropertyChangedEventArgs(propertyName));
    }

    #endregion

    public static MyClass BuildRandom()
    {
        if (_random == null)
            _random = new Random((int)DateTime.Now.Ticks);

        var myclass = new MyClass
        {
            Id = Guid.NewGuid().ToString("N"),
            SomeRandomNumber = _random.Next(1, 100)
        };

        return myclass;
    }
}
``` 
  
Running the code shows that indeed the AddRange works well to speed up inserts. I was getting about 15 to 25 seconds on the Slow Data and from 3 to 0.09 seconds on the Fast Data. It was interesting to note that if the Slow button was pressed a second time it actually took longer than the first click, but the Fast button had the opposite affect.