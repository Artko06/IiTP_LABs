using System.Text.Json;
using _353503_KOKHAN_LAB8.Consts;

namespace _353503_KOKHAN_LAB8.Services
{
    public class StreamService<T>
    {
        private readonly Semaphore _semaphore = new(ConstData.MAX_OF_THREADS, ConstData.MAX_OF_THREADS);

        public async Task WriteToStreamAsync(Stream stream, IEnumerable<T> data, IProgress<(string, int)> progress)
        {
            _semaphore.WaitOne();
            progress.Report(("Start writing to stream", Thread.CurrentThread.ManagedThreadId));

            await JsonSerializer.SerializeAsync(stream, data);
            //await Task.Delay(ConstData.DELAY);
            //Thread.Sleep(200);

            progress.Report(("Finished writing to stream", Thread.CurrentThread.ManagedThreadId));
            _semaphore.Release();
        }

        public async Task CopyFromStreamAsync(Stream stream, string filename, IProgress<(string, int)> progress)
        {
            _semaphore.WaitOne();

            await using (var fileStream = File.Create(filename))
            {
                stream.Position = 0;
                
                progress.Report(("Start reading from stream", Thread.CurrentThread.ManagedThreadId));

                await stream.CopyToAsync(fileStream).ConfigureAwait(true);

                progress.Report(("Finished reading from stream", Thread.CurrentThread.ManagedThreadId));
            }

            _semaphore.Release();
        }

        public async Task<int?> GetStatisticsAsync(string fileName, Func<T, bool> filter)
        {
            await using var stream = File.OpenRead(fileName);

            var data = await JsonSerializer.DeserializeAsync<IEnumerable<T>>(stream);
            
            return data?.AsParallel().Where(filter).Count();
        }
    }
}