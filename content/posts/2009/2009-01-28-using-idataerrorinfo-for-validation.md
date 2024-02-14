---
title: 'Using IDataErrorInfo for Validation'
date: 2009-01-28T10:10:00.000-08:00
draft: false
xurlx: /2009/01/using-idataerrorinfo-for-validation.html
tags: 
- .Net
- Rant
---

I read an article on [CodeProject](http://www.codeproject.com) titled [Total View Validation](http://www.codeproject.com/KB/WPF/GlobalWPFValidation.aspx) where the author complains that IDataErrorInfo is inadequate for WPF validation.  The assumption he makes is that all the validation code needs to go into the IDataErrorInfo.this[string] property as such:  
  
```csharp
public string this[string name]  
{  
    get  
    {  
        string result = null;  
        if (name == "Age")  
            if (this.age < 0 || this.age > 150)  
                result = "Age must not be less than 0 or greater than 150.";  
        return result;  
    }  
}  
```  
  
This should not be the way that IDataErrorInfo is used as it puts business logic in the model, as the author points out. His solution though was to create another mechanism to notifiy the user of validation errors and to not use IDataErrorInfo at all.  
  
A solution I would propose though would be to follow how DataTables and DataTables use IDataErrorInfo by implementing methods to set and clear the objects error information:  
  
```csharp  
public class SomeClass : IDataErrorInfo
{
    public string Name { get; set; }

    public string Age { get; set; }

    #region Data Error Info  

    private Dictionary<string, string> _propertyErrors;

    private void InitDataErrorInfo()
    {
        var properties = this.GetType().GetProperties();

        // This will act as an overall error message for the entire object.  
        _propertyErrors = new Dictionary<string, string>
        {
            {this.GetHashCode().ToString(CultureInfo.InvariantCulture), string.Empty}
        };

        foreach (var propertyInfo in properties)
            _propertyErrors.Add(propertyInfo.Name, string.Empty);
    }

    public void ClearDataErrorInfo()
    {
        foreach (var property in _propertyErrors.Keys)
            _propertyErrors[property] = string.Empty;
    }

    public void ClearDataErrorInfo(string propertyName)
    {
        AssertThisHasPropertyWithName(propertyName);
        _propertyErrors[propertyName] = string.Empty;
    }

    public void SetError(string error)
    {
        SetError(this.GetHashCode().ToString(CultureInfo.InvariantCulture), error);
    }

    public void SetError(string propertyName, string error)
    {
        AssertThisHasPropertyWithName(propertyName);
        _propertyErrors[propertyName] = string.Format("{0}{1}{2}", _propertyErrors[propertyName]
        , Environment.NewLine, error);
    }

    public string this[string propertyName]
    {
        get
        {
            AssertThisHasPropertyWithName(propertyName);
            return _propertyErrors[propertyName];
        }
    }

    public string Error
    {
        get
        {
            var errors = new StringBuilder();
            foreach (
            var propertyError in
            _propertyErrors.Where(propertyError => !string.IsNullOrEmpty(propertyError.Value)))
                errors.AppendLine(propertyError.Value);

            return errors.ToString().Trim();
        }
    }

    protected void AssertThisHasPropertyWithName(string propertyName)
    {
        if (!_propertyErrors.ContainsKey(propertyName))
        {
            throw new ArgumentException(string.Format("No property named {0} on {1}."
            , propertyName, this.GetType().FullName));
        }
    }

    #endregion
}
``` 
  
Note that there is no validation here, only reporting if the object has errors. Using this takes advantage of the already existing validation notification built into WPF as well as WinForms.