namespace WEB_353503_KOKHAN.UI.Services.CategoryService
{
    public class MemoryCategoryService : ICategoryService
    {
        public Task<ResponseData<List<Category>>> GetCategoryListAsync()
        {
            var categories = new List<Category>
            {
                new Category {Id=1, Name="Седан", NormalizedName="sedan-cars"},
                new Category {Id=2, Name="Хэтчбек", NormalizedName="hatchback-cars"},
                new Category {Id=3, Name="Универсал", NormalizedName="station-wagon-cars"},
                new Category {Id=4, Name="Кроссовер", NormalizedName="сuv-cars"},
                new Category {Id=5, Name="Внедорожник", NormalizedName="suv-cars"},
                new Category {Id=6, Name="Купе", NormalizedName="coupe-cars"},
                new Category {Id=7, Name="Кабриолет", NormalizedName="cabriolet-cars"},
                new Category {Id=8, Name="Минивэн", NormalizedName="mpv-cars"},
                new Category {Id=9, Name="Лифтбек", NormalizedName="liftback-cars"},
                new Category {Id=10, Name="Пикап", NormalizedName="pick-up-cars"},
            };

            var result = ResponseData<List<Category>>.Success(categories);

            return Task.FromResult(result);
        }
    }
}
