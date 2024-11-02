using _353503_KOKHAN_LAB2.Collections;
using _353503_KOKHAN_LAB2.Entities;

internal class Program
{
    private static void Main(string[] args)
    {
        Company.Notify += Journal.LogEvent;
        Company.SuccessOrder += (Order order) => Console.WriteLine($"{order.ClientOrder.Name} successfully placed the order for {order.TariffOrder.Price}\n");

        MyCustomCollection<Client> listOfClients = [new("Artem"), new("Vasya")];

        Tariff tariff1 = new Tariff("Basic", 100);
        Tariff tariff2 = new Tariff("Premium", 200);

        Order order1 = new Order(listOfClients[0], tariff1);
        Order order2 = new Order(listOfClients[1], tariff2);
        Order order3 = new Order(listOfClients[1], tariff1);

        Company.AddClient(listOfClients[0]);
        Company.AddClient(listOfClients[1]);
        Company.AddOrder(order1);
        Company.AddOrder(order2);
        Company.AddOrder(order3);
        Company.AddTariff(tariff1);
        Company.AddTariff(tariff2);

        Console.WriteLine($"Total orders: {Company.CountOrders()}");
        Console.WriteLine($"Sum of orders: {Company.SumAllOrders<Order>()}");
        Console.WriteLine($"Sum of basic tariff: {Company.SumOrdersWithIdenticalTariff(tariff1)}");
        Console.WriteLine($"Sum orders for {listOfClients[1].Name}: {Company.SumOrdersForClient(listOfClients[1])}\n");

        Company.InputAllOrders();
        Company.InputAllTariffs();
        Company.InputAllClients();
        Company.AddClient(new Client("Pasha"));
        Journal.OutputAllEvents();

        try
        {
            Console.WriteLine("Let's try to address by negative index\n");
            Console.WriteLine(listOfClients[-1].Name);
        }
        catch (IndexOutOfRangeException)
        {
            Console.WriteLine("Invalid index -1\n");
        }

        try
        {
            Console.WriteLine("Let's try to address by out of range index\n");
            Console.WriteLine(listOfClients[2].Name);
        }
        catch (IndexOutOfRangeException)
        {
            Console.WriteLine("Invalid index 2\n");
        }
    }
}