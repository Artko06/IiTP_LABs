using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc.Rendering;

namespace WEB_353503_KOKHAN.UI.Areas.Admin.Pages.Cars
{
    public class CreateModel : PageModel
    {
        private readonly ICarService _carService;
        private readonly ICategoryService _categoryService;

        public CreateModel(
            ICarService carService,
            ICategoryService categoryService)
        {
            _carService = carService;
            _categoryService = categoryService;
        }

        public async Task<IActionResult> OnGetAsync()
        {
            var categoriesResponse = await _categoryService.GetCategoryListAsync();
            ViewData["CategoryId"] = new SelectList(categoriesResponse.Data, "Id", "Name");
            return Page();
        }

        [BindProperty]
        public Car Car { get; set; } = new Car();

        [BindProperty] 
        public IFormFile? Image { get; set; }

        public async Task<IActionResult> OnPostAsync()
        {
            if (!ModelState.IsValid)
            {
                var categoriesResponse = await _categoryService.GetCategoryListAsync();
                ViewData["CategoryId"] = new SelectList(categoriesResponse.Data, "Id", "Name");
                return Page();
            }

            var result = await _carService.CreateProductAsync(Car, Image);

            if (!result.Successfull)
            {
                var categoriesResponse = await _categoryService.GetCategoryListAsync();
                ViewData["CategoryId"] = new SelectList(categoriesResponse.Data, "Id", "Name");
                return Page();
            }

            return RedirectToPage("./Index");
        }
    }
}