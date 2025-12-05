using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc.Rendering;
using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.UI.Services.CarService;

namespace WEB_353503_KOKHAN.UI.Areas.Admin.Pages.Cars
{
    public class EditModel : PageModel
    {
        private readonly ICarService _carService;
        private readonly ICategoryService _categoryService;

        public EditModel(
            ICarService carService,
            ICategoryService categoryService)
        {
            _carService = carService;
            _categoryService = categoryService;
        }

        [BindProperty]
        public Car Car { get; set; } = default!;

        [BindProperty] 
        public IFormFile? Image { get; set; }

        public async Task<IActionResult> OnGetAsync(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var response = await _carService.GetProductByIdAsync(id.Value);
            if (!response.Successfull || response.Data == null)
            {
                return NotFound();
            }
            Car = response.Data;

            var categoriesResponse = await _categoryService.GetCategoryListAsync();
            ViewData["CategoryId"] = new SelectList(categoriesResponse.Data, "Id", "Name");

            return Page();
        }

        public async Task<IActionResult> OnPostAsync()
        {
            if (!ModelState.IsValid)
            {
                var categoriesResponse = await _categoryService.GetCategoryListAsync();
                ViewData["CategoryId"] = new SelectList(categoriesResponse.Data, "Id", "Name");
                return Page();
            }

            try
            {
                await _carService.UpdateProductAsync(Car.Id, Car, Image);
            }
            catch (Exception ex)
            {
                ModelState.AddModelError("", $"Ошибка обновления: {ex.Message}");
                var categoriesResponse = await _categoryService.GetCategoryListAsync();
                ViewData["CategoryId"] = new SelectList(categoriesResponse.Data, "Id", "Name");
                return Page();
            }

            return RedirectToPage("./Index");
        }
    }
}