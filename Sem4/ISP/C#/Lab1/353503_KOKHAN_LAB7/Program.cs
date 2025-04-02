using _353503_KOKHAN_LAB7;


internal class Program
{
    static IntegralCalculator ManageSin(byte maxThreads)
    {
        var sin = new IntegralCalculator(maxThreads);

        sin.CalculationProgress += (sender, eventArgs) =>
        {
            string progressLine = new string('=', eventArgs.Percent / 10);
            if(eventArgs.Percent != 0)
            {
                progressLine += '>';
            }

            Console.WriteLine($"Поток {eventArgs.ThreadId}: [{progressLine}] {eventArgs.Percent}%");
        };

        sin.CalculationCompleted += (sender, eventArgs) =>
        {
            Console.WriteLine(
                $"Поток {eventArgs.ThreadId}: Завершен с результатом: {eventArgs.Result}: За {eventArgs.ticks} тиков");
        };

        return sin;
    }

    static void RunTwoThreads(IntegralCalculator sin)
    {
        var thread1 = new Thread(sin.Calculate);
        var thread2 = new Thread(sin.Calculate);

        thread1.Priority = ThreadPriority.Highest;
        thread2.Priority = ThreadPriority.Lowest;
        
        thread1.Start();
        thread2.Start();

        thread1.Join();
        thread2.Join();
    }

    static void RunNThreads(IntegralCalculator sin, byte n)
    {
        var threads = new List<Thread>();

        for (byte i = 0; i < n; ++i)
        {
            threads.Add(new Thread(sin.Calculate));
        }

        foreach (var thread in threads)
        {
            thread.Start();
        }
    }

    static void Main(string[] args)
    {
        var funSin = ManageSin(1);
        RunTwoThreads(funSin);

        Console.WriteLine("\nFor n threads:\n");
        Thread.Sleep(1000);

        RunNThreads(funSin, 5); // for 5 threads
    }
}