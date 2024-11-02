using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal class NoDiscount : IPrice
    {
        private double price;

        public NoDiscount(double price) => 
            this.price = price;

        public double GetPrice() => price;
    }
}