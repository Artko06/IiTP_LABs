using System.Numerics;

namespace _353503_KOKHAN_LAB1.Entities
{
    public class Order : IAdditionOperators<Order, Order, Order>
    {
        private Tariff tariffOrder;

        private Client clientOrder;

        public Tariff TariffOrder{get{return tariffOrder;}}

        public Client ClientOrder{get{return clientOrder;}}

        public Order(Client client, Tariff tariff)
        {
            tariffOrder = tariff;
            clientOrder = client;
            ++client.CountOfOrders;
        }

        public Order()
        {
            tariffOrder = new Tariff(" ", 0);
            clientOrder = new Client(" ");
        }

        public static Order operator +(Order left, Order right)
        {
            return new Order(new Client("temp"), new Tariff("temp", left.tariffOrder.Price + right.tariffOrder.Price));
        }
    }
}