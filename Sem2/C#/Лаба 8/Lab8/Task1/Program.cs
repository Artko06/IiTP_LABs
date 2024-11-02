using System.Collections.Concurrent;
using Task1.Classes;

namespace Task1
{
    file class Program
    {
        static void Main(string[] args)
        {
            TrackCompany myCompany = new TrackCompany();

            myCompany.AddTariff(new Tariff("Быстрый", new NoDiscount(30)));
            myCompany.AddTariff(new Tariff("Ускоренный люкс", new AbsDiscount(200, 35)));
            myCompany.AddTariff(new Tariff("Экономный", new AbsDiscount(20, 15)));
            myCompany.AddTariff(new Tariff("Безопасный", new AbsDiscount(30, 5)));
            myCompany.AddTariff(new Tariff("Люкс", new NoDiscount(100)));

            Console.WriteLine("Выведем список всех тарифов:");
            myCompany.GetAllTariff();

            Console.WriteLine("Выведем самый дешёвый тариф: ");
            myCompany.GetTheCheapestTariiff();
        }
    }
}