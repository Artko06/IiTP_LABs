using System;
using System.Diagnostics;
using System.Threading;

namespace _353503_KOKHAN_LAB7
{
    public class IntegralCalculator(byte maxThread)
    {
        private const double _step = 0.00000001;
        private readonly Semaphore _semaphore = new Semaphore(maxThread, maxThread); // limit for count of threads

        public class CalculationCompletedEventArgs(double result, long ticks, int threadId) : EventArgs
        {
            public double Result { get; } = result;
            public long ticks { get; } = ticks;
            public int ThreadId { get; } = threadId;
        }

        public class CalculationProgressEventArgs(int percent, int threadId) : EventArgs
        {
            public int Percent { get; } = percent;
            public int ThreadId { get; } = threadId;
        }

        public event EventHandler<CalculationCompletedEventArgs> CalculationCompleted = delegate { };
        public event EventHandler<CalculationProgressEventArgs> CalculationProgress = delegate { };

        public void Calculate()
        {
            _semaphore.WaitOne();
            var stopwatch = new Stopwatch();
            var threadId = Thread.CurrentThread.ManagedThreadId;
            stopwatch.Start();

            double result = 0;

            for (double x = 0; x <= 1; x += _step)
            {
                result += Math.Sin(x) * _step;

                // Input progress
                if (x % 0.2 < _step)
                    CalculationProgress?.Invoke(this, new CalculationProgressEventArgs((int)(x * 100), threadId));
            }

            stopwatch.Stop();
            CalculationCompleted?.Invoke(this, new CalculationCompletedEventArgs(result, stopwatch.ElapsedTicks, threadId));
            _semaphore.Release();
        }
    }
}