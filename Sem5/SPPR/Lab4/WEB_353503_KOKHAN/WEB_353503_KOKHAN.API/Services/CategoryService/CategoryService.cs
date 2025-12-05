using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;

namespace WEB_353503_KOKHAN.API.Services.CategoryService
{
    public class CategoryService : ICategoryService
    {
        private readonly AppDbContext _context;

        public CategoryService(AppDbContext context)
        {
            _context = context;
        }

        public async Task<ResponseData<List<Category>>> GetCategoryListAsync()
        {
            var categories = await _context.Categories.ToListAsync();
            return ResponseData<List<Category>>.Success(categories);
        }
    }
}
