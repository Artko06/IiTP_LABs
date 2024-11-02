using System.Reflection.Metadata;
using System.Runtime.Intrinsics.X86;
using _353503_KOKHAN_LAB4.Comparers;
using _353503_KOKHAN_LAB4.Entities;
using _353503_KOKHAN_LAB4.FileClasses;

internal class Program
{
    private static void Main(string[] args)
    {
        const string nameFolder = "KOKHAN_Lab4";

        CreateDirectory(nameFolder);
        
        GenerateFiles(nameFolder);

        GetFiles(nameFolder);

        List<StoreGoods> storeGoods = new(){new StoreGoods("Mineral water", false, 666),
                                            new StoreGoods("Apples", true, 45),
                                            new StoreGoods("Bread", true, 33),
                                            new StoreGoods("Milk", false, 189),
                                            new StoreGoods("Chocolate", false, 120),
                                         };
        IEnumerable<StoreGoods> storeGoodsEmpty;

        File.Create(Path.Combine(nameFolder, "test.txt")).Dispose();

        FileService fileService = new();
        fileService.SaveData(storeGoods, Path.Combine(nameFolder, "test.txt"));

        storeGoodsEmpty = fileService.ReadFile(Path.Combine(nameFolder, "test.txt"));

        Console.WriteLine("Items from file:");
        foreach (StoreGoods item in storeGoodsEmpty)
        {
            Console.WriteLine($"{item.Name}, {item.NumbersOfGoods}, {item.NotEnoughGoods}");
        }
        Console.WriteLine();

        Console.WriteLine("Items sorted by Name from file:");
        foreach (StoreGoods item in storeGoodsEmpty.OrderBy(x => x, new MyCustomComparer()))
        {
            Console.WriteLine($"{item.Name}, {item.NumbersOfGoods}, {item.NotEnoughGoods}");
        }
        Console.WriteLine();

        Console.WriteLine("Items sorted by Numbers from file:");
        foreach (StoreGoods item in storeGoodsEmpty.OrderBy(x => x.NumbersOfGoods))
        {
            Console.WriteLine($"{item.Name}, {item.NumbersOfGoods}, {item.NotEnoughGoods}");
        }
        Console.WriteLine();
    }


    private static void CreateDirectory(string name)
    {
        if (Directory.Exists(name))
        {
            Console.WriteLine($"All files in folder \"{name}\" was deleted");
            Directory.Delete(name, true);
        }

        Console.WriteLine($"Folder \"{name}\" was created\n");
        Directory.CreateDirectory(name);
    }

    private static void GenerateFiles(string nameFolder)
    {
        string[] extensionsFiles = { ".txt", ".rtf", ".dat", ".inf" };
        Random random = new Random();

        for (int i = 0; i < 10; ++i)
        {
            File.Create(Path.Combine(nameFolder, Path.GetRandomFileName() + extensionsFiles[random.Next(0, extensionsFiles.Length)])).Dispose();
        }
        Console.WriteLine("10 Files were created\n");
    }

    private static void GetFiles(string nameFolder)
    {
        string[] namesFiles = Directory.GetFiles(nameFolder);

        Console.WriteLine("Files in folder:");
        foreach (string file in namesFiles)
        {
            Console.WriteLine($"File: {Path.GetFileName(file)} have extension {Path.GetExtension(file)}");
        }
        Console.WriteLine();
    }
}