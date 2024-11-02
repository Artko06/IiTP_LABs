using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal class Laptop : ComputerEquipment
    {   
        private protected double squareTouchpad { get; set; }

        public Laptop(string manufacturer, string modelEquipment,
            double sizeScreen, double price, double squareTouchpad) : base(manufacturer, modelEquipment,
            sizeScreen, price)
        {
            Console.WriteLine("Вызван конструктор класса Laptop\n");
            this.squareTouchpad = squareTouchpad;
        }

        public override void TakeInfoEquipment()
        {
            Console.WriteLine($"1. Производитель: {manufacturer}\n" +
                              $"2. Модель: {modelEquipment}\n" +
                              $"3. Размеры экрана: {sizeScreen}\n" +
                              $"4. Цена: {price}$\n" + 
                              $"5. Размеры тачпада: {squareTouchpad}\n");
        }

        public override void LaunchEquipment() => 
            Console.WriteLine("Launch laptop\n");

        public override void СompletionWork() =>
            Console.WriteLine("Close the laptop lid\n");
    }
}