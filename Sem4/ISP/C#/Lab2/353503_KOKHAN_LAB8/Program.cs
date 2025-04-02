using _353503_KOKHAN_LAB8.Entities;
using _353503_KOKHAN_LAB8.Services;
using _353503_KOKHAN_LAB8.Consts;

internal class Program
{
    private static async Task Main(string[] args)
    {
        List<CarShowroom> carShowrooms = new List<CarShowroom>();

        var random = new Random();
        for (short i = 0; i < ConstData.COUNT_OF_ITERATIONS; ++i)
        {
            CarShowroom carShowroom = new(i, $"Car showroom №{i + 1}", random.Next(ConstData.MAX_RANDOM_VALUE));

            carShowrooms.Add(carShowroom);
        }

        StreamService<CarShowroom> streamService = new StreamService<CarShowroom>();
        MemoryStream memoryStream = new MemoryStream();

        var progress = new Progress<(string, int)>(report =>
        {
            Console.WriteLine($"Thread {report.Item2}: {report.Item1}");

            if (report.Item1.Contains("Finished"))
            {
                Console.WriteLine('\n');
            }
        });

        System.Console.WriteLine($"Primary thread : {Thread.CurrentThread.ManagedThreadId}");
        // WriteToStreamAsync
        var writeTask = streamService.WriteToStreamAsync(memoryStream, carShowrooms, progress);
        writeTask.GetAwaiter().GetResult();

        Thread.Sleep(ConstData.SLEEP_MAIN_FLOW);

        // CopyFromStreamAsync
        var copyTask = Task.Run(() => streamService.CopyFromStreamAsync(memoryStream, ConstData.FILE_NAME, progress));
        copyTask.GetAwaiter().GetResult();

        Thread.Sleep(ConstData.SLEEP_MAIN_FLOW);

        // GetStatisticsAsync
        int? stats = await streamService.GetStatisticsAsync(ConstData.FILE_NAME, (carShowroom) =>
                                                                                carShowroom.CountWhereCapacityEngineMoreTwoLitres > 100);
        Console.WriteLine($"Number of cars where engine capacity > 2 liters: {stats}");
    }
}