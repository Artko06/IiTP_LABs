using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal abstract class ComputerEquipment
    {
        private protected string manufacturer { get; set; }
        private protected string modelEquipment { get; set;}
        private protected double sizeScreen { get; set; }
        private protected double price { get; set; }

        private protected ComputerEquipment(string manufacturer, string modelEquipment,
            double sizeScreen, double price )
        {
            Console.WriteLine("Вызван конструктор базового класса\n");

            this.manufacturer = manufacturer;
            this.modelEquipment = modelEquipment;
            this.sizeScreen = sizeScreen;
            this.price = price;
        }

        public abstract void TakeInfoEquipment();

        public virtual void LaunchEquipment() =>
            Console.WriteLine("Launch device\n");

        public virtual void СompletionWork() =>
            Console.WriteLine("Completion of work(parent method)\n");
    }
}