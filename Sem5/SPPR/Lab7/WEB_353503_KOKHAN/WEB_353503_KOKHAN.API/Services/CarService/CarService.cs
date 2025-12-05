using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;

namespace WEB_353503_KOKHAN.API.Services.CarService
{
    public class CarService: ICarService
    {
        private readonly AppDbContext _context;
        private readonly int _maxPageSize = 20;

        public CarService(AppDbContext context)
        {
            _context = context;
        }

        public async Task<ResponseData<ListModel<Car>>> GetProductListAsync(string? categoryNormalizedName, int pageNo = 1, int pageSize = 3)
        {
            if (pageSize > _maxPageSize)
                pageSize = _maxPageSize;

            var query = _context.Cars.Include(m => m.Category).AsQueryable();
            var dataList = new ListModel<Car>();

            if (!string.IsNullOrEmpty(categoryNormalizedName))
            {
                query = query.Where(d => d.Category.NormalizedName.Equals(categoryNormalizedName));
            }

            var count = await query.CountAsync();
            if (count == 0)
            {
                return ResponseData<ListModel<Car>>.Success(dataList);
            }

            int totalPages = (int)Math.Ceiling(count / (double)pageSize);
            if (pageNo > totalPages)
                return ResponseData<ListModel<Car>>.Error("No such page");

            dataList.Items = await query
                .OrderBy(d => d.Id)
                .Skip((pageNo - 1) * pageSize)
                .Take(pageSize)
                .ToListAsync();

            dataList.CurrentPage = pageNo;
            dataList.TotalPages = totalPages;
            return ResponseData<ListModel<Car>>.Success(dataList);
        }

        public async Task<ResponseData<Car>> GetProductByIdAsync(int id)
        {
            var product = await _context.Cars.FindAsync(id);
            if (product == null)
            {
                return ResponseData<Car>.Error("Product not found");
            }

            return ResponseData<Car>.Success(product);
        }

        public async Task UpdateProductAsync(int id, Car car)
        {
            if (id != car.Id)
            {
                throw new ArgumentException("Product ID mismatch");
            }

            _context.Entry(car).State = EntityState.Modified;
            await _context.SaveChangesAsync();
        }

        public async Task DeleteProductAsync(int id)
        {
            var product = await _context.Cars.FindAsync(id);
            if (product == null)
            {
                throw new KeyNotFoundException("Product not found");
            }

            _context.Cars.Remove(product);
            await _context.SaveChangesAsync();
        }

        public async Task<ResponseData<Car>> CreateProductAsync(Car car)
        {
            _context.Cars.Add(car);
            await _context.SaveChangesAsync();
            return ResponseData<Car>.Success(car);
        }

        public async Task<ResponseData<string>> SaveImageAsync(int id, IFormFile formFile)
        {
            if (formFile == null || formFile.Length == 0)
            {
                return ResponseData<string>.Error("No file uploaded.");
            }

            var imagePath = Path.Combine("wwwroot", "images");
            if (!Directory.Exists(imagePath))
            {
                Directory.CreateDirectory(imagePath);
            }

            var fileName = $"{id}_{Path.GetFileName(formFile.FileName)}";
            var filePath = Path.Combine(imagePath, fileName);

            using (var stream = new FileStream(filePath, FileMode.Create))
            {
                await formFile.CopyToAsync(stream);
            }

            var url = $"{Path.Combine("images", fileName)}";
            return ResponseData<string>.Success(url);
        }
    }
}
