using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal class AbsDiscount : IPrice
    {
        private double price;
        private double discount;
        
        public AbsDiscount(double price, double discount)
        {
            this.price = price;
            this.discount = discount;
        }

        public double GetPrice() => 
            price * (100 - discount) / 100;
    }
}