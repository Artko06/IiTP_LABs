namespace _353503_KOKHAN_LAB3.Entities
{
    public class Client
    {
        private string name;

        private double countOfOrders;

        private double sumOrders;

        public string Name { get { return name; } }
        public double CountOfOrders { get { return countOfOrders; } set { countOfOrders = value; } }
        public double SumOrders { get { return sumOrders; } set { sumOrders = value; } }

        public Client(string name)
        {
            this.name = name;
            countOfOrders = 0;
            sumOrders = 0;
        }
    }
}