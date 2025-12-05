using Microsoft.AspNetCore.Mvc;

namespace WEB_353503_KOKHAN.UI.Controllers
{
    public class CarController : Controller
    {
        private readonly ICarService _carService;
        private readonly ICategoryService _categoryService;
        private readonly IConfiguration _configuration;

        public CarController(
            ICarService carService,
            ICategoryService categoryService,
            IConfiguration configuration)
        {
            _carService = carService;
            _categoryService = categoryService;
            _configuration = configuration;
        }

        public async Task<IActionResult> Index(string? category, int pageNo = 1)
        {
            var categoriesResponse = await _categoryService.GetCategoryListAsync();
            if (!categoriesResponse.Successfull)
            {
                return NotFound("Categories could not be loaded.");
            }

            ViewBag.Categories = categoriesResponse.Data;

            var productResponse = await _carService.GetProductListAsync(category, pageNo);
            if (!productResponse.Successfull)
            {
                return NotFound(productResponse.ErrorMessage);
            }

            var currentCategory = category;
            if (string.IsNullOrEmpty(currentCategory))
            {
                currentCategory = "Все Автомобили";
            }
            else
            {
                var selectedCategory = categoriesResponse.Data.FirstOrDefault(c => c.NormalizedName == currentCategory);
                currentCategory = selectedCategory?.Name ?? "Все Автомобили";
            }

            ViewBag.CurrentCategory = currentCategory;
            ViewBag.CurrentPage = pageNo;
            ViewBag.TotalPages = productResponse.Data.TotalPages;

            return View(productResponse.Data);
        }
    }
}
