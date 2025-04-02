using Lab4.Entities.Http;

namespace Lab4.Services.Http
{
    public interface IRateService
    {
        Task<IEnumerable<Rate>> GetRates(DateTime date);
    }
}
