using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;

namespace WEB_353503_KOKHAN.UI.Areas.Admin.Pages.Cars
{
    public class DeleteModel : PageModel
    {
        private readonly ICarService _carService;

        public DeleteModel(ICarService carService)
        {
            _carService = carService;
        }

        [BindProperty]
        public Car Car { get; set; } = new Car();

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
            return Page();
        }

        public async Task<IActionResult> OnPostAsync(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            await _carService.DeleteProductAsync(id.Value);

            return RedirectToPage("./Index");
        }
    }
}
