using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using WEB_353503_KOKHAN.Domain.Entities;

namespace WEB_353503_KOKHAN.Domain.Models
{
    public class Cart
    {
        public Dictionary<int, CartItem> CartItems { get; set; } = new();

        public virtual void AddToCart(Car car)
        {
            if (CartItems.ContainsKey(car.Id))
            {
                CartItems[car.Id].Quantity++;
            }
            else
            {
                CartItems[car.Id] = new CartItem { Car = car, Quantity = 1 };
            }
        }

        public virtual void RemoveFromCart(int carId)
        {
            if (CartItems.ContainsKey(carId))
            {
                if (CartItems[carId].Quantity > 1)
                {
                    CartItems[carId].Quantity--;
                }
                else
                {
                    CartItems.Remove(carId);
                }
            }
        }

        public virtual void RemoveItem(int carId)
        {
            CartItems.Remove(carId);
        }

        public virtual void ClearAll()
        {
            CartItems.Clear();
        }

        [JsonIgnore]
        public int Count => CartItems.Sum(item => item.Value.Quantity);

        [JsonIgnore]
        public double TotalPrice => CartItems.Sum(item => item.Value.TotalPrice);
    }
}
