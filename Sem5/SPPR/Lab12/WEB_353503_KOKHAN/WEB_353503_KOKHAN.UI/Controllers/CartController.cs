using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using WEB_353503_KOKHAN.UI.Extensions;

namespace WEB_353503_KOKHAN.UI.Controllers
{
    [Authorize]
    public class CartController : Controller
    {
        private readonly ICarService _carService;
        private readonly Cart _cart;

        public CartController(ICarService carService, Cart cart)
        {
            _carService = carService;
            _cart = cart;
        }

        [Route("[controller]/[action]")]
        public IActionResult Index()
        {
            return View(_cart);
        }

        [Route("[controller]/[action]/{id:int}")]
        public async Task<IActionResult> Add(int id, string returnUrl)
        {
            var carResult = await _carService.GetProductByIdAsync(id);
            if (carResult.Successfull)
            {
                _cart.AddToCart(carResult.Data);
            }

            return Redirect(returnUrl ?? "/");
        }

        [Route("[controller]/[action]/{id:int}")]
        public IActionResult Remove(int id, string returnUrl)
        {
            _cart.RemoveFromCart(id);
            return Redirect(returnUrl ?? "/Cart");
        }

        [Route("[controller]/[action]/{id:int}")]
        public IActionResult RemoveItem(int id, string returnUrl)
        {
            _cart.RemoveItem(id);
            return Redirect(returnUrl ?? "/Cart");
        }

        [Route("[controller]/[action]")]
        public IActionResult Clear(string returnUrl)
        {
            _cart.ClearAll();
            return Redirect(returnUrl ?? "/");
        }
    }
}