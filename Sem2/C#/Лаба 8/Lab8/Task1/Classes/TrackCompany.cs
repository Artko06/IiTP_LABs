using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using Task1.Classes;
using static Task1.Classes.Tariff;

namespace Task1.Classes
{
    internal class TrackCompany
    {
        private SortedList<double, string> LstTariffs;

        public TrackCompany() => 
            LstTariffs = new SortedList<double, string>();

        public void AddTariff(Tariff tariff) =>
            LstTariffs.Add(tariff.GetPrice(), tariff.nameTariff);

        public void GetAllTariff()
        {
            foreach (KeyValuePair<double, string> entry in LstTariffs)
            {
                Console.WriteLine("Стоимость тарифа: {0}$, Название тарифа: {1}\n", entry.Key, entry.Value);
            }
        }

        public void GetTheCheapestTariiff() =>
           Console.WriteLine("Стоимость тарифа: {0}$, Название тарифа: {1}\n", LstTariffs.Keys[0], LstTariffs.Values[0]);
    }
}