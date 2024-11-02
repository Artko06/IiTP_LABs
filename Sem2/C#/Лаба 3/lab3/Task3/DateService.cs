using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task3
{
    static public class DateService
    {
        static public string GetDay(string? date)
        {
            DateTime dateTime = new DateTime();

            while (true)
            {
                Console.WriteLine("Input date YYYY-MM-DD:");
                date = Console.ReadLine();
                
                if(DateTime.TryParse(date, out dateTime))
                {
                    dateTime = DateTime.Parse(date);
                    break;
                }
            }

            return dateTime.DayOfWeek.ToString();
        }

        static public int GetDaysSpan(int day, int month, int year)
        {
            DateTime currentDate = DateTime.Now;
            DateTime inputDate = new DateTime(year, month, day);

            TimeSpan timeSpan = inputDate - currentDate;
            
            return Math.Abs(timeSpan.Days);
        }
    }
}