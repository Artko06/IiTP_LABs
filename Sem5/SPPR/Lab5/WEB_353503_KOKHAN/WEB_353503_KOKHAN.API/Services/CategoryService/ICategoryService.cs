using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;

namespace WEB_353503_KOKHAN.API.Services.CategoryService
{
    public interface ICategoryService
    {
        Task<ResponseData<List<Category>>> GetCategoryListAsync();
    }
}
