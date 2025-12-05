using Microsoft.AspNetCore.Mvc;

namespace WEB_353503_KOKHAN.UI.Views.Shared.Components.Cart
{
    public class CartViewComponent : ViewComponent
    {
        public IViewComponentResult Invoke()
        {
            return View();
        }
    }
}
