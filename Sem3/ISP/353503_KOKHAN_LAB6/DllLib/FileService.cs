using System.Text.Json;

using LAB6.Entities;
using LAB6.Interface;

namespace DllLib;

public class FileService : IFileService<Employee>
{
    public IEnumerable<Employee> ReadFile(string fileName)
    {
        return JsonSerializer.Deserialize<List<Employee>>(File.ReadAllText(fileName)) ?? Enumerable.Empty<Employee>();
    }

    public void SaveData(IEnumerable<Employee> data, string fileName)
    {
        File.WriteAllText(fileName, JsonSerializer.Serialize(data));
    }
}
