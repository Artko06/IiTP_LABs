namespace _353503_KOKHAN_LAB4.Interfaces
{
    public interface IFileService<T> 
    {
        IEnumerable<T> ReadFile(string fileName);
        void SaveData(IEnumerable<T> data, string fileName);
    }
}