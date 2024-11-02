using Task1.Classes;

namespace Task1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            TrackCompany myCompany = TrackCompany.GetInstance("Cargo");
            myCompany.AddClient("Artyom", "1234");
            myCompany.AddClient("Timka", "timka1958");

            Console.WriteLine("\nList of tariffs:");
            Tariff.GetAllTariffs();

            Console.WriteLine("\nList of clients:");
            myCompany.GetAllClients();

            Console.WriteLine("\nLet's make orders:");
            myCompany.OrderTransportation("Artyom", "1234", 5.04, Tariff.Tariffs.economical);
            myCompany.OrderTransportation("Artyom", "1234", 128.99, Tariff.Tariffs.luxury);
            myCompany.OrderTransportation("Timka", "timka1958", 666.666666, Tariff.Tariffs.safe);

            Console.WriteLine("\nLet's look at the total amount of orders:");
            myCompany.GetTotalSumOrders("Artyom", "1234");

            Console.WriteLine("\nLet's check out classes:");
            myCompany.AddClient("Artyom", "simplePassoword1234");
            myCompany.AddClient("Artyom", "1234");
            myCompany.OrderTransportation("Artyom", "123", 51, Tariff.Tariffs.fast);
            myCompany.OrderTransportation("Artem", "1234", 51, Tariff.Tariffs.fast);
            myCompany.GetTotalSumOrders("Artyom", "12345");
            myCompany.GetTotalSumOrders("Arteeeeeeeem", "1234");
        }
    }
}