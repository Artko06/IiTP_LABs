namespace _353503_KOKHAN_LAB2.Entities
{
    public class Tariff
    {
        private string name;

        private double price;

        public string Name { get{ return name; }}

        public double Price { get{ return price; }}

        public Tariff(string name, double price)
        {
            this.name = name;
            this.price = price;
        }
    }
}