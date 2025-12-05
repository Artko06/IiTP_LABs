using Microsoft.AspNetCore.Mvc.RazorPages;

namespace WEB_353503_KOKHAN.UI.Areas.Admin.Pages.Cars
{
    public class IndexModel : PageModel
    {
        private readonly ICarService _carService;

        public IndexModel(ICarService carService)
        {
            _carService = carService;
        }

        public IList<Car> Car { get; set; } = new List<Car>();
        public int TotalPages { get; set; } = 1;
        public int CurrentPage { get; set; } = 1;

        public async Task OnGetAsync(int pageNo = 1)
        {
            CurrentPage = pageNo;
            var response = await _carService.GetProductListAsync(null, pageNo);

            if (response.Successfull && response.Data != null)
            {
                Car = response.Data.Items;
                TotalPages = response.Data.TotalPages;
            }
            else
            {
                Car = new List<Car>();
                TotalPages = 1;
            }
        }
    }
}