using _1Task.Classes;

namespace _1Task
{
    internal class Program
    {
        static void CorrectInput(out double value)
        {
            string? str = null;

            while (true)
            {
                str = Console.ReadLine();

                if (double.TryParse(str, out value))
                {
                    value = double.Parse(str);
                    break;
                }
                Console.WriteLine("re-Input value: ");
            }
        }

        static void Main(string[] args)
        {
                string? nameCompany = null, str = null;
                double costTariff = 0, toneWeight = 0;

                    Console.WriteLine("Input name your company:");
                    nameCompany = Console.ReadLine();

                Console.WriteLine("Input cost tariff:");
                CorrectInput(out costTariff);

                Console.WriteLine("Input the weight of the transported:");
                CorrectInput(out toneWeight);

                TrackCompany myCompany = TrackCompany.GetInstance(nameCompany, toneWeight, costTariff);

            while (true)
            {
                Console.WriteLine("Select method or other number to exit:\n" +
                    "1 - Get name my company\n2 - Get cost tariff\n" +
                    "3 - Get weight of the transported\n4 - Get profit my company\n" +
                    "5 - Upp tariff\n6 - Down tariff"); 

                    str = Console.ReadLine();

                    if (str != "1" && str != "2" && str != "3" && str != "4" &&
                        str != "5" && str != "6") Environment.Exit(0);

                switch (int.Parse(str))
                {
                    case 1:
                        {
                            Console.WriteLine("Name company: " + myCompany.nameCompany + "\n");
                            break;
                        }

                    case 2: 
                        {
                            Console.WriteLine("The cost of tariff: " + 
                                myCompany.tariff.costTone + "\n");
                            break;
                        }
                    case 3:
                        {
                            Console.WriteLine("The weight of the transported: " +
                                myCompany.toneWeight + "\n");
                            break;
                        }
                    case 4:
                        {
                            Console.WriteLine("Profit company: " + 
                                myCompany.profitCompany() + "\n");
                            break;
                        }
                    case 5:
                        {   double value = 0;

                            Console.WriteLine("Input value:");
                            CorrectInput(out value);

                            myCompany.UppCost(value);
                            break;
                        }
                    case 6:
                        {
                            double value = 0;

                            Console.WriteLine("Input value:");
                            CorrectInput(out value);

                            myCompany.DownCost(value);
                            break;
                        }
                }
            }
        }
    }
}
