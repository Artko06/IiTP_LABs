using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal sealed class SmartWatch : Smartphone
    {
        private string materialWatchband { get; set; }
        public SmartWatch(string manufacturer, string modelEquipment,
            double sizeScreen, double price, double countSimSlots, string materialWatchband) : 
                base(manufacturer, modelEquipment,
                sizeScreen, price, countSimSlots)
        {
            Console.WriteLine("Вызван конструктор класса SmartWatch\n");
            this.materialWatchband = materialWatchband;
        }

        public override void TakeInfoEquipment()
        {
            Console.WriteLine($"1. Производитель: {manufacturer}\n" +
                              $"2. Модель: {modelEquipment}\n" +
                              $"3. Размеры экрана: {sizeScreen}\n" +
                              $"4. Цена: {price}$\n" +
                              $"5. Количество слотов для сим карт: {countSimSlots}\n" +
                              $"6. Материал ремешка часов: {materialWatchband}\n");
        }

        public new void СompletionWork() => 
            base.СompletionWork();
    }
}