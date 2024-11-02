namespace _353503_KOKHAN_LAB1.Entities
{
    public class Client
    {
        private string name;

        private double countOfOrders;

        public string Name { get{ return name; }}
        public double CountOfOrders { get{ return countOfOrders; } set{ countOfOrders = value; } }

        public Client(string name)
        {
            this.name = name;
            countOfOrders = 0;
        }
    }
}