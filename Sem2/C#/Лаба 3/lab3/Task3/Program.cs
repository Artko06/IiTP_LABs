using System.ComponentModel;

namespace Task3
{
    internal class Program
    {
        static bool CheckDate(int dayValue, int monthValue, int yearValue)
        {
            if (dayValue < 1 || dayValue > 31 || monthValue < 1 || monthValue > 12
                || yearValue < 1) return false;

            else if (yearValue % 4 != 0 && monthValue == 2 && dayValue > 28) return false;

            else if (yearValue % 4 == 0 && monthValue == 2 && dayValue < 30) return true;

            else if ((monthValue == 4 || monthValue == 6 || monthValue == 9 || monthValue == 11) &&
                dayValue > 30) return false;

            return true;
        }

        static void CorrectInput(out int value)
        {
            string? str = null;

            while (true)
            {
                str = Console.ReadLine();

                if (int.TryParse(str, out value))
                {
                    value = int.Parse(str);
                    break;
                }
                Console.WriteLine("re-Input value: ");
            }
        }

        static void Main(string[] args)
        {
            bool replay = true;

            while (replay)
            {
                string? str = null;
                int day, month, year;

                Console.WriteLine("Choise function:\n1 - DateService.GetDay\n" +
                    "2 - DateService.GetDaysSpan");
                while(true)
                {
                    str = Console.ReadLine();

                    if (str == "1" || str == "2") break;

                    else Environment.Exit(0);
                }

                switch(int.Parse(str)){
                    case 1:
                        {
                            Console.WriteLine("Day of week: " + DateService.GetDay(str));
                            break;
                        }
                    case 2:
                        {
                            while (true)
                            {
                                Console.WriteLine("Input day of year: ");
                                CorrectInput(out day);

                                Console.WriteLine("Input month of year: ");
                                CorrectInput(out month);

                                Console.WriteLine("Input year: ");
                                CorrectInput(out year);

                                if (CheckDate(day, month, year)) break;
                            }

                            Console.WriteLine("Between dates: " + 
                                DateService.GetDaysSpan(day, month, year));
                            break;
                        }
                }

                Console.WriteLine("Input 'e' to end or another symbol to continue");
                str = Console.ReadLine();

                if (str == "e") replay = false;

            }
        }
    }
}