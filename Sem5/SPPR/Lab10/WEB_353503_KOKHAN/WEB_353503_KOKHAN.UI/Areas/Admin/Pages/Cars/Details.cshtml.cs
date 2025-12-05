using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;

namespace WEB_353503_KOKHAN.UI.Areas.Admin.Pages.Cars
{
    public class DetailsModel : PageModel
    {
        private readonly ICarService _carService;

        public DetailsModel(ICarService carService)
        {
            _carService = carService;
        }

        public Car Car { get; set; } = new Car();

        public async Task<IActionResult> OnGetAsync([FromQuery] int? id)
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
    }
}
