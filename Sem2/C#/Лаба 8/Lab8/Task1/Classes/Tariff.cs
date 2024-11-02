using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal class Tariff
    {
        private IPrice typeTariff;
        public string nameTariff { get; set; }

        public Tariff(string nameTariff, IPrice typeTariff)
        {
            this.nameTariff = nameTariff;
            this.typeTariff = typeTariff;
        }

        public double GetPrice() 
        {
            return typeTariff.GetPrice();
        }
    }
}