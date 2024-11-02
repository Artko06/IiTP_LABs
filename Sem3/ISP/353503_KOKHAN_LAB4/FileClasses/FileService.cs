using _353503_KOKHAN_LAB4.Entities;
using _353503_KOKHAN_LAB4.Interfaces;

namespace _353503_KOKHAN_LAB4.FileClasses
{
    public class FileService : IFileService<StoreGoods>
    {
        public IEnumerable<StoreGoods> ReadFile(string fileName)
        {
            using (BinaryReader reader = new BinaryReader(File.Open(fileName, FileMode.Open)))
            {
                while(reader.PeekChar() != -1)
                {
                    string name;
                    int numbersOfGoods;
                    bool notEnoughGoods;

                    try
                    {
                        name = reader.ReadString();
                        numbersOfGoods = reader.ReadInt32();
                        notEnoughGoods = reader.ReadBoolean();

                    }
                    catch(Exception ex)
                    {
                        Console.WriteLine($"Error reading from file once item: {ex.Message}");
                        continue;
                    }

                    yield return new StoreGoods(name, notEnoughGoods, numbersOfGoods);
                }
            }
        }

        public void SaveData(IEnumerable<StoreGoods> data, string fileName)
        {
            using (BinaryWriter writer = new BinaryWriter(File.Open(fileName, FileMode.Create)))
            {
                foreach(StoreGoods item in data)
                {
                    if(item is not null)
                    {
                        try
                        {
                            writer.Write(item.Name ?? string.Empty);
                            writer.Write(item.NumbersOfGoods);
                            writer.Write(item.NotEnoughGoods);
                        }
                        catch(Exception ex)
                        {
                            Console.WriteLine($"Error writing to file once item: {ex.Message}");
                            continue;
                        }
                    }
                }
            }
        }
    }
}