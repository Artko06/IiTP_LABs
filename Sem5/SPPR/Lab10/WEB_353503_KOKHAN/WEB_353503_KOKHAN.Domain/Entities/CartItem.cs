using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WEB_353503_KOKHAN.Domain.Entities
{
    public class CartItem
    {
        public Car Car { get; set; } = new();
        public int Quantity { get; set; } = 1;

        public double TotalPrice => Car.Price * Quantity;
    }
}
