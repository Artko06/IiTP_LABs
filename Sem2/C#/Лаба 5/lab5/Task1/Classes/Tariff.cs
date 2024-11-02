using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal class Tariff
    {
        public enum Tariffs : byte
        {
            economical = 10,
            safe = 20,
            fast = 25,
            luxury = 50
        }

        public static void GetAllTariffs()
        {
            string[] enumTariffs = Enum.GetNames(typeof(Tariffs));

            for (byte i = 0; i < enumTariffs.Length; ++i)
            {
                Console.WriteLine($"{i + 1}. " + enumTariffs.ElementAt(i));
            }
        }
    }
}