
using WEB_353503_KOKHAN.UI.Services.CategoryService;

namespace WEB_353503_KOKHAN.UI.Services.CarService
{
    public class MemoryCarService : ICarService
    {
        private readonly IConfiguration _configuration;
        private List<Car> _cars;
        private List<Category> _categories;
        private readonly ICategoryService _categoryService;

        public MemoryCarService(
            IConfiguration config,
            ICategoryService categoryService)
        {
            _configuration = config;
            _categories = categoryService.GetCategoryListAsync().Result.Data;
            SetupData();
        }

        private void SetupData()
        {
            _cars = new List<Car>
            {
                new Car
                {
                    Id = 1,
                    Name = "Granta Sedan 2025",
                    Description = "Современный, технологичный, стремительный и стильный — автомобиль, каким он должен быть",
                    Price = 33900,
                    ImagePath = "images/cars/granta_sedan.png",
                    ImageMimeType = "image/png",
                    Category = _categories.Find(c => c.NormalizedName.Equals("sedan-cars"))
                },
                new Car
                {
                    Id = 2,
                    Name = "Granta Liftback 2025",
                    Description = "Динамичный стиль и обтекаемые формы признак автомобиля со спортивным характером." +
                    " Но лифтбек Granta еще и потрясающе вместителен",
                    Price = 34900,
                    ImagePath = "images/cars/granta_liftback.png",
                    ImageMimeType = "image/png",
                    Category = _categories.Find(c => c.NormalizedName.Equals("liftback-cars"))
                },
                new Car
                {
                    Id = 3,
                    Name = "Vesta SW 2025",
                    Description = "Стильный, вместительный, комфортный универсал. Современное сочетание практичности и элегантности",
                    Price = 49500,
                    ImagePath = "images/cars/vesta_station_wagon.png",
                    ImageMimeType = "image/png",
                    Category = _categories.Find(c => c.NormalizedName.Equals("station-wagon-cars"))
                },
                new Car
                {
                    Id = 4,
                    Name = "Vesta Sedan 2025",
                    Description = "Вместимость и оснащенность, элегантность и практичность — Новая Vesta превосходна во всем",
                    Price = 47500,
                    ImagePath = "images/cars/vesta_sedan.png",
                    ImageMimeType = "image/png",
                    Category = _categories.Find(c => c.NormalizedName.Equals("sedan-cars"))
                },
                new Car
                {
                    Id = 5,
                    Name = "Largus 2025",
                    Description = "Путешественник, партнер по бизнесу и член семьи — это LADA Largus: комфортный, яркий и функциональный",
                    Price = 47000,
                    ImagePath = "images/cars/largus_station_wagon.png",
                    ImageMimeType = "image/png",
                    Category = _categories.Find(c => c.NormalizedName.Equals("station-wagon-cars"))
                },
                new Car
                {
                    Id = 6,
                    Name = "Niva Legend 2025",
                    Description = "LADA Niva Legend — узнаваемый стиль и прославленная проходимость",
                    Price = 34580,
                    ImagePath = "images/cars/niva_suv.png",
                    ImageMimeType = "image/png",
                    Category = _categories.Find(c => c.NormalizedName.Equals("suv-cars"))
                },
                new Car
                {
                    Id = 7,
                    Name = "Niva Travel 2025",
                    Description = "LADA Niva Travel — это мощь полного привода и постоянная готовность к приключениям. К приключениям с комфортом",
                    Price = 43050,
                    ImagePath = "images/cars/niva_travel_suv.png",
                    ImageMimeType = "image/png",
                    Category = _categories.Find(c => c.NormalizedName.Equals("suv-cars"))
                },
            };
        }
        public async Task<ResponseData<ListModel<Car>>> GetProductListAsync(string? categoryNormalizedName, int pageNo = 1)
        {
            // Получаем размер страницы из конфигурации
            var itemsPerPage = _configuration.GetValue<int>("PageSettings:ItemsPerPage");

            var filteredItems = _cars
                .Where(m => categoryNormalizedName == null || (m.Category != null && m.Category.NormalizedName.Equals(categoryNormalizedName)))
                .ToList();

            int totalItems = filteredItems.Count;
            int totalPages = (int)Math.Ceiling((double)totalItems / itemsPerPage);

            var pagedItems = filteredItems
                .Skip((pageNo - 1) * itemsPerPage)
                .Take(itemsPerPage)
                .ToList();

            var result = new ListModel<Car>
            {
                Items = pagedItems,
                CurrentPage = pageNo,
                TotalPages = totalPages
            };

            return ResponseData<ListModel<Car>>.Success(result);
        }

        public Task<ResponseData<Car>> GetProductByIdAsync(int id)
        {
            throw new NotImplementedException();
        }

        public Task DeleteProductAsync(int id)
        {
            throw new NotImplementedException();
        }

        public Task<ResponseData<Car>> CreateProductAsync(Car product, IFormFile? formFile)
        {
            throw new NotImplementedException();
        }

        public Task UpdateProductAsync(int id, Car motorcycle, IFormFile? formFile)
        {
            throw new NotImplementedException();
        }
    }
}
