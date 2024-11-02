namespace _353503_KOKHAN_LAB4.Entities
{
    public class StoreGoods
    {
        private string? _name;
        private int _numbersOfGoods;
        private bool _notEnoughGoods;

        public string? Name{get{return _name;} set{_name = value;}}

        public int NumbersOfGoods { get{return _numbersOfGoods;} set{_numbersOfGoods = value;}}

        public bool NotEnoughGoods { get{return _notEnoughGoods;} set{_notEnoughGoods = value;}}

        public StoreGoods(string name, bool notEnoughGoods, int numbersOfGoods)
        {
            Name = name;
            NumbersOfGoods = numbersOfGoods;
            NotEnoughGoods = notEnoughGoods;
        }

        public StoreGoods(string name, bool notEnoughGoods) : this(name, notEnoughGoods, 0)
        {   }
    }
}