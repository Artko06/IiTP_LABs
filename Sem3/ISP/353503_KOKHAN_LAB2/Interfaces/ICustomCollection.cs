namespace _353503_KOKHAN_LAB2.Interfaces
{
    public interface ICustomCollection<T>
    {
        T this[int index] { get; set; }
        int Count { get; }

        void Reset();
        void Next();
        void Add(T item);
        void Remove(T item);
        T Current();
        T RemoveCurrent();
    }
}