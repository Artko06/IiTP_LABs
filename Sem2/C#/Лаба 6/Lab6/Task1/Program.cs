using System;
using Task1.Classes;

namespace Task1
{
    file class Program
    {
        static void Main(string[] args)
        {
            Laptop firsrMacLaptop = new Laptop("Apple", "PowerBook", 10, 6500, 5);

            firsrMacLaptop.TakeInfoEquipment();
            firsrMacLaptop.LaunchEquipment();
            Console.WriteLine("СompletionWork for Laptop:");
            firsrMacLaptop.СompletionWork();

            Laptop victus = new Laptop("HP", "Victus 16", 16, 2735.535, 12.5);

            victus.TakeInfoEquipment();
            victus.LaunchEquipment();
            Console.WriteLine("СompletionWork for Laptop:");
            victus.СompletionWork();

            Smartphone mySmartphone = new Smartphone("Honor", "20", 6.1, 369, 2);

            mySmartphone.TakeInfoEquipment();
            mySmartphone.LaunchEquipment();
            Console.WriteLine("СompletionWork for SmartPhone:");
            mySmartphone.СompletionWork();

            Smartphone badSmartphone = new Smartphone("Xiaomi", "14", 6.36, 999, 2);

            badSmartphone.TakeInfoEquipment();
            badSmartphone.LaunchEquipment();
            Console.WriteLine("СompletionWork for SmartPhone:");
            badSmartphone.СompletionWork();

            SmartWatch myWatch = new SmartWatch("Huawei", "Watch fit 2", 1.88, 100, 0, "Artificial leather");

            myWatch.TakeInfoEquipment();
            myWatch.LaunchEquipment();
            Console.WriteLine("СompletionWork for SmartWatch:");
            myWatch.СompletionWork();
        }
    }
}