using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;

namespace WEB_353503_KOKHAN.API.Services.CarService
{
    public interface ICarService
    {
        Task<ResponseData<ListModel<Car>>> GetProductListAsync(string? categoryNormalizedName, int pageNo = 1, int pageSize = 3);
        Task<ResponseData<Car>> GetProductByIdAsync(int id);
        Task UpdateProductAsync(int id, Car car);
        Task DeleteProductAsync(int id);
        Task<ResponseData<Car>> CreateProductAsync(Car car);
        Task<ResponseData<string>> SaveImageAsync(int id, IFormFile formFile);
    }
}
