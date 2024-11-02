using System.Numerics;
using _353503_KOKHAN_LAB2.Collections;

namespace _353503_KOKHAN_LAB2.Entities
{
    public static class Company 
    {
        public delegate void ClientHandler(Client client);
        public static event ClientHandler? Notify;

        public delegate void OrderHandler(Order order);
        public static event OrderHandler? SuccessOrder;

        private static MyCustomCollection<Client> listOfClients = new MyCustomCollection<Client>();

        private static MyCustomCollection<Order> listOfOrders = new MyCustomCollection<Order>();

        private static MyCustomCollection<Tariff> listOfTariffs = new MyCustomCollection<Tariff>();

        public static void AddClient(Client client)
        {
            listOfClients.Add(client);
            Notify?.Invoke(client);
        }

        public static void RemoveClient(Client client) => listOfClients.Remove(client);

        public static void AddOrder(Order order) 
        {
            listOfOrders.Add(order);
            SuccessOrder?.Invoke(order);
        }

        public static void RemoveOrder(Order order) => listOfOrders.Remove(order);

        public static void AddTariff(Tariff tariff) => listOfTariffs.Add(tariff);

        public static void RemoveTariff(Tariff tariff) => listOfTariffs.Remove(tariff);

        public static int CountOrders() => listOfOrders.Count;

        public static double SumAllOrders<T>() 
            where T : IAdditionOperators<Order, Order, Order>, new()
        {
            Order result = new();
            foreach (var order in listOfOrders)
            {
                result += order;
            }
            return result.TariffOrder.Price;
        }

        public static double SumOrdersWithIdenticalTariff(Tariff tariff)
        {
            Order total = new Order(new Client("temp"), new Tariff("temp", 0));
            foreach (var order in listOfOrders)
            {
                if (tariff.Equals(order.TariffOrder)) total += order;
            }
            return total.TariffOrder.Price;
        }

        public static double SumOrdersForClient(Client client)
        {
            Order total = new Order(new Client("temp"), new Tariff("temp", 0));
            foreach (var order in listOfOrders)
            {
                if (client.Equals(order.ClientOrder)) total += order;
            }
            return total.TariffOrder.Price;
        }

        public static void InputAllOrders()
        {   
            Console.WriteLine("Orders: ");
            for (int i = 0; i < listOfOrders.Count; ++i)
            {
                Console.WriteLine($"{i+1}. Client: {listOfOrders[i].ClientOrder.Name}, Tariff: {listOfOrders[i].TariffOrder.Name}");
            }
            Console.WriteLine();
        }

        public static void InputAllTariffs()
        {
            Console.WriteLine("Tariffs: ");
            for (int i = 0; i < listOfTariffs.Count; ++i)
            {
                Console.WriteLine($"{i + 1}. Name: {listOfTariffs[i].Name}, Price: {listOfTariffs[i].Price}");
            }
            Console.WriteLine();
        }

        public static void InputAllClients()
        {
            Console.WriteLine("Clients: ");
            for (int i = 0; i < listOfClients.Count; ++i)
            {
                Console.WriteLine($"{i + 1}. Name: {listOfClients[i].Name}");
            }
            Console.WriteLine();
        }
    }
}