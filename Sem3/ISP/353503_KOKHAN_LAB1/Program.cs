using _353503_KOKHAN_LAB1.Entities;

internal class Program
{
    private static void Main(string[] args)
    {
        Client client1 = new Client("Artem");
        Client client2 = new Client("Vasya");

        Tariff tariff1 = new Tariff("Basic", 100);
        Tariff tariff2 = new Tariff("Premium", 200);

        Order order1 = new Order(client1, tariff1);
        Order order2 = new Order(client2, tariff2);
        Order order3 = new Order(client2, tariff1);

        Company.AddClient(client1);
        Company.AddClient(client2);
        Company.AddOrder(order1);
        Company.AddOrder(order2);
        Company.AddOrder(order3);
        Company.AddTariff(tariff1);
        Company.AddTariff(tariff2);

        Console.WriteLine($"Total orders: {Company.CountOrders()}");
        Console.WriteLine($"Sum of orders: {Company.SumAllOrders<Order>()}");
        Console.WriteLine($"Sum of basic tariff: {Company.SumOrdersWithIdenticalTariff(tariff1)}");
        Console.WriteLine($"Sum orders for Vasya: {Company.SumOrdersForClient(client2)}\n");

        Company.InputAllOrders();
        Company.InputAllTariffs();
        Company.InputAllClients();
    }
}