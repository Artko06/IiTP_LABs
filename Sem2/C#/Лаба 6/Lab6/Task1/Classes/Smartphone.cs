using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal class Smartphone : ComputerEquipment
    {
        private protected double countSimSlots { get; set; }
        public Smartphone(string manufacturer, string modelEquipment,
            double sizeScreen, double price, double countSimSlots) : base(manufacturer, modelEquipment,
            sizeScreen, price)
        {
            Console.WriteLine("Вызван конструктор класса Smartphone\n");
            this.countSimSlots = countSimSlots;
        }

        public override void TakeInfoEquipment()
        {
            Console.WriteLine($"1. Производитель: {manufacturer}\n" +
                              $"2. Модель: {modelEquipment}\n" +
                              $"3. Размеры экрана: {sizeScreen}\n" +
                              $"4. Цена: {price}$\n" +
                              $"5. Количество слотов для сим карт: {countSimSlots}\n");
        }

        public new void СompletionWork()
        {
            Console.WriteLine("The main button was pressed(Smartphone method)");
            base.СompletionWork();
        }
    }
}