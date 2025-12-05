using Microsoft.AspNetCore.Mvc;

namespace WEB_353503_KOKHAN.UI.Views.Shared.Components.Cart
{
    public class CartViewComponent : ViewComponent
    {
        private readonly Domain.Models.Cart _cart;

        public CartViewComponent(Domain.Models.Cart cart)
        {
            _cart = cart;
        }

        public IViewComponentResult Invoke()
        {
            return View(_cart);
        }
    }
}
