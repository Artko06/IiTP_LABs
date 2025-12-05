using WEB_353503_KOKHAN.Domain.Entities;

namespace WEB_353503_KOKHAN.BlazorWasm.Services
{
    public interface IDataService
    {
        event Action DataLoaded;

        List<Category> Categories { get; set; }

        List<Car> Cars { get; set; }

        bool Success { get; set; }

        string ErrorMessage { get; set; }

        int TotalPages { get; set; }

        int CurrentPage { get; set; }

        Category? SelectedCategory { get; set; }

        Task GetProductListAsync(int pageNo = 1);

        Task GetCategoryListAsync();
    }
}
