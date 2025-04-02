using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.UI.ValueConvertors
{
    class DateToStringConvertor : IValueConverter
    {
        public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
        {
            return value is DateTime date ? date.ToString("dd/MM/yyyy") : string.Empty;
        }

        public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
        {
            return value is string str ? DateTime.Parse(str) : DateTime.MinValue;
        }
    }
}
