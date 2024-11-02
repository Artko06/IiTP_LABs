using _353503_KOKHAN_LAB3.Entities;

internal class Program
{
    private static void Main(string[] args)
    {
        Company.Notify += Journal.LogEvent;
        Company.SuccessOrder += (Order order) => Console.WriteLine($"{order.ClientOrder.Name} successfully placed the order for {order.PriceTariff}\n");

        List<Client> listOfClients = [new("Artem"), new("Vasya")];

        Company.AddTariff("Basic", 100);
        Company.AddTariff("Premium", 200);

        Order order1 = new Order(listOfClients[0], "Basic", Company.GetTariff("Basic"));
        Order order2 = new Order(listOfClients[1], "Premium", Company.GetTariff("Premium"));
        Order order3 = new Order(listOfClients[1], "Basic", Company.GetTariff("Basic"));
        Order order4 = new Order(listOfClients[1], "Basic", Company.GetTariff("Basic"));


        Company.AddClient(listOfClients[0]);
        Company.AddClient(listOfClients[1]);
        Company.AddOrder(order1);
        Company.AddOrder(order2);
        Company.AddOrder(order3);
        Company.AddOrder(order4);

        Console.WriteLine($"Total orders: {Company.CountOrders()}");
        Console.WriteLine($"Sum of orders: {Company.SumAllOrders<Order>()}");
        Console.WriteLine($"Sum of basic tariff: {Company.SumOrdersWithIdenticalTariff("Basic")}");
        Console.WriteLine($"Sum orders for {listOfClients[1].Name}: {Company.SumOrdersForClient(listOfClients[1])}\n");
        
        Console.WriteLine("Sorted orders:");
        foreach (var item in Company.InputAllOrders())
        {
            Console.WriteLine($"Client: {item.ClientOrder.Name}, Tariff: {item.NameTariff}, Price: {item.PriceTariff}");
        }
        Console.WriteLine();

        Console.WriteLine($"Client with max sum orders: {Company.ClientWithMaxSum()}\n");

        Console.WriteLine($"Count of clients with sum bigger then 300: {Company.CountClientBiggerThenSomeSum(300)}\n");

        Console.WriteLine($"Info about each of tariff at the client {listOfClients[1].Name}:");
        foreach (var item in Company.SumClientOnEachOfTariff(listOfClients[1]))
        {
            Console.WriteLine($"Tariff: {item.Key}, Sum: {item.Value}");
        }
        Console.WriteLine();

        Company.InputAllTariffs();
        Company.InputAllClients();
        Company.AddClient(new Client("Pasha"));
        Journal.OutputAllEvents();
    }
}