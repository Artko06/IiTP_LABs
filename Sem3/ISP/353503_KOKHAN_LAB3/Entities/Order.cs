using System.Numerics;

namespace _353503_KOKHAN_LAB3.Entities
{
    public class Order : IAdditionOperators<Order, Order, Order>
    {
        private string nameTariff;
        private double priceTariff;

        private Client clientOrder;

        public string NameTariff{get{return nameTariff;}}

        public double PriceTariff { get { return priceTariff; } }

        public Client ClientOrder{get{return clientOrder;}}

        public Order(Client client, string nameTariff, double priceTariff)
        {
            this.nameTariff = nameTariff;
            this.priceTariff = priceTariff;
            clientOrder = client;
            ++client.CountOfOrders;
        }

        public Order()
        {
            nameTariff = " ";
            priceTariff = 0;
            clientOrder = new Client(" ");
        }

        public static Order operator +(Order left, Order right)
        {
            return new Order(new Client("temp"), "temp", left.PriceTariff + right.PriceTariff);
        }
    }
}