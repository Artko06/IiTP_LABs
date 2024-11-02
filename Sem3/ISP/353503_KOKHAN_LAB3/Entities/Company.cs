using System.Numerics;
using System.Security.Cryptography;

namespace _353503_KOKHAN_LAB3.Entities
{
    public static class Company 
    {
        public delegate void ClientHandler(Client client);
        public static event ClientHandler? Notify;

        public delegate void OrderHandler(Order order);
        public static event OrderHandler? SuccessOrder;

        private static List<Client> listOfClients = new List<Client>();

        private static List<Order> listOfOrders = new List<Order>();

        private static Dictionary<string, double> listOfTariffs = new Dictionary<string, double>();

        public static void AddClient(Client client)
        {
            listOfClients.Add(client);
            Notify?.Invoke(client);
        }

        public static void RemoveClient(Client client) => listOfClients.Remove(client);

        public static void AddOrder(Order order) 
        {
            listOfOrders.Add(order);
            order.ClientOrder.SumOrders += order.PriceTariff;
            SuccessOrder?.Invoke(order);
        }

        public static void RemoveOrder(Order order) => listOfOrders.Remove(order);

        public static void AddTariff(string name, double price) => listOfTariffs.Add(name, price);

        public static double GetTariff(string name) => listOfTariffs[name];

        public static void RemoveTariff(string name) => listOfTariffs.Remove(name);

        public static int CountOrders() => listOfOrders.Count;

        public static double SumAllOrders<T>() 
            where T : IAdditionOperators<Order, Order, Order>, new()
        {
            Order result = new();
            foreach (var order in listOfOrders)
            {
                result += order;
            }
            return result.PriceTariff;
        }

        public static double SumOrdersWithIdenticalTariff(string nameTariff)
        {
            Order total = new Order(new Client("temp"), nameTariff, 0);
            foreach (var order in listOfOrders)
            {
                if (nameTariff.Equals(order.NameTariff)) total += order;
            }
            return total.PriceTariff;
        }

        public static double SumOrdersForClient(Client client) => client.SumOrders;

        public static List<Order> InputAllOrders() => (from order in listOfOrders 
                                                        orderby order.PriceTariff descending 
                                                        select order).ToList();

        public static string ClientWithMaxSum() => (from client in listOfClients orderby client.SumOrders select client).Last().Name;

        public static int CountClientBiggerThenSomeSum(double sum) => listOfClients.Aggregate(0, (count, client) => client.SumOrders > sum ? ++count : count);

        public static Dictionary<string, double> SumClientOnEachOfTariff(Client client)
        {
            return listOfOrders
            .Where(order => order.ClientOrder == client)
            .GroupBy(order => order.NameTariff)
            .ToDictionary(
                group => group.Key,
                group => group.Sum(order => order.PriceTariff)
            );
        }

        public static void InputAllTariffs()
        {
            Console.WriteLine("Tariffs: ");
            foreach (var item in listOfTariffs)
            {
                Console.WriteLine($"Name: {item.Key}, Price: {item.Value}");
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