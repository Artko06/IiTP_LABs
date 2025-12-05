using WEB_353503_KOKHAN.UI.Extensions;

namespace WEB_353503_KOKHAN.UI.Services
{
    public class SessionCart : Cart
    {
        private readonly IHttpContextAccessor _httpContextAccessor;

        public SessionCart(IHttpContextAccessor httpContextAccessor)
        {
            _httpContextAccessor = httpContextAccessor;
            LoadFromSession();
        }

        private ISession Session => _httpContextAccessor.HttpContext.Session;

        private void LoadFromSession()
        {
            var savedCart = Session.Get<Cart>("cart");
            if (savedCart != null && savedCart.CartItems != null)
            {
                CartItems = savedCart.CartItems;
            }
        }

        private void SaveToSession()
        {
            Session.Set("cart", this);
        }

        public override void AddToCart(Car car)
        {
            base.AddToCart(car);
            SaveToSession();
        }

        public override void RemoveFromCart(int carId)
        {
            base.RemoveFromCart(carId);
            SaveToSession();
        }

        public override void RemoveItem(int carId)
        {
            base.RemoveItem(carId);
            SaveToSession();
        }

        public override void ClearAll()
        {
            base.ClearAll();
            SaveToSession();
        }
    }
}