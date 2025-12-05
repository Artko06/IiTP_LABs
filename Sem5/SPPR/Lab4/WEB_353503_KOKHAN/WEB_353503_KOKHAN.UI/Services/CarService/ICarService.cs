namespace WEB_353503_KOKHAN.UI.Services.CarService
{
    public interface ICarService
    {
        public Task<ResponseData<ListModel<Car>>> GetProductListAsync(string? categoryNormalizedName, int pageNo = 1);
        public Task<ResponseData<Car>> GetProductByIdAsync(int id);
        public Task UpdateProductAsync(int id, Car product, IFormFile? formFile);
        public Task DeleteProductAsync(int id);
        public Task<ResponseData<Car>> CreateProductAsync(Car product, IFormFile? formFile);
    }
}
