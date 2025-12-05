using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Options;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using WEB_353503_KOKHAN.UI.HelperClasses;
using WEB_353503_KOKHAN.UI.Models;
using WEB_353503_KOKHAN.UI.Services.Authentication;
using WEB_353503_KOKHAN.UI.Services.FileService;

namespace WEB_353503_KOKHAN.UI.Controllers
{
    public class AccountController : Controller
    {
        private readonly IFileService _fileService;
        private readonly HttpClient _httpClient;
        private readonly ITokenAccessor _tokenAccessor;
        private readonly KeycloakData _keycloakData;

        public AccountController(
            IFileService fileService,
            HttpClient httpClient,
            ITokenAccessor tokenAccessor,
            IOptions<KeycloakData> options)
        {
            _fileService = fileService;
            _httpClient = httpClient;
            _tokenAccessor = tokenAccessor;
            _keycloakData = options.Value;
        }

        // GET: /Account/Login
        [HttpGet]
        public IActionResult Login(string? returnUrl = null)
        {
            if (User.Identity?.IsAuthenticated == true)
            {
                return RedirectToAction("Index", "Home");
            }

            ViewData["ReturnUrl"] = returnUrl;
            return View();
        }

        // POST: /Account/Login
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult LoginPost(string? returnUrl = null)
        {
            returnUrl ??= Url.Content("~/");

            var properties = new AuthenticationProperties
            {
                RedirectUri = returnUrl,

                AllowRefresh = true
            };

            properties.Parameters.Add("prompt", "login");

            return Challenge(properties, "keycloak");
        }

        [HttpGet]
        public async Task<IActionResult> Logout()
        {
            await HttpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);
            await HttpContext.SignOutAsync("keycloak",
                new AuthenticationProperties { RedirectUri = Url.Action("Index", "Home") });

            return RedirectToAction("Index", "Home");
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> LogoutPost()
        {
            await HttpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);
            await HttpContext.SignOutAsync("keycloak",
                new AuthenticationProperties { RedirectUri = Url.Action("Index", "Home") });

            return RedirectToAction("Index", "Home");
        }

        // GET: /Account/Register
        [HttpGet]
        public IActionResult Register()
        {
            return View(new RegisterUserViewModel());
        }

        // POST: /Account/Register
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Register(RegisterUserViewModel model)
        {
            if (!ModelState.IsValid)
            {
                return View(model);
            }

            try
            {
                // Сохраняем аватар если загружен
                var avatarUrl = "/images/default-profile-picture.png";
                if (model.Avatar != null && model.Avatar.Length > 0)
                {
                    avatarUrl = await _fileService.SaveFileAsync(model.Avatar);
                }

                // Получаем токен клиента для административного API
                await _tokenAccessor.SetAuthorizationHeaderAsync(_httpClient, true);

                // Подготавливаем данные для Keycloak
                var newUser = new CreateUserModel
                {
                    Username = model.Email,
                    Email = model.Email,
                    Attributes = new Dictionary<string, string[]>
                    {
                        ["avatar"] = new[] { avatarUrl }
                    },
                    Credentials = new List<UserCredentials>
                    {
                        new UserCredentials { Value = model.Password }
                    }
                };

                // Сериализуем в JSON
                var serializerOptions = new JsonSerializerOptions
                {
                    PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
                    DefaultIgnoreCondition = System.Text.Json.Serialization.JsonIgnoreCondition.WhenWritingNull
                };

                var userData = JsonSerializer.Serialize(newUser, serializerOptions);
                var content = new StringContent(userData, Encoding.UTF8, "application/json");

                // Отправляем запрос к Keycloak Admin API
                var requestUrl = $"{_keycloakData.Host}/admin/realms/{_keycloakData.Realm}/users";
                var response = await _httpClient.PostAsync(requestUrl, content);

                if (response.IsSuccessStatusCode)
                {
                    TempData["SuccessMessage"] = "Регистрация прошла успешно! Теперь вы можете войти в систему.";
                    return RedirectToAction("Login");
                }
                else
                {
                    var errorContent = await response.Content.ReadAsStringAsync();
                    ModelState.AddModelError("", $"Ошибка регистрации: {response.StatusCode} - {errorContent}");
                }
            }
            catch (Exception ex)
            {
                ModelState.AddModelError("", $"Ошибка при регистрации: {ex.Message}");
            }

            return View(model);
        }

        // GET: /Account/AccessDenied
        [HttpGet]
        public IActionResult AccessDenied()
        {
            return View();
        }

        // GET: /Account/UserInfo
        [HttpGet]
        public IActionResult UserInfo()
        {
            if (User.Identity?.IsAuthenticated != true)
            {
                return RedirectToAction("Login");
            }

            ViewBag.UserName = User.Identity.Name;
            ViewBag.Claims = User.Claims.Select(c => new { c.Type, c.Value }).ToList();

            return View();
        }

        [HttpGet]
        public IActionResult LoginRedirect()
        {
            // Простой редирект на страницу входа
            return RedirectToAction("Login");
        }
    }

    public class CreateUserModel
    {
        [JsonPropertyName("attributes")]
        public Dictionary<string, string[]> Attributes { get; set; } = new();

        [JsonPropertyName("username")]
        public string Username { get; set; } = string.Empty;

        [JsonPropertyName("email")]
        public string Email { get; set; } = string.Empty;

        [JsonPropertyName("enabled")]
        public bool Enabled { get; set; } = true;

        [JsonPropertyName("emailVerified")]
        public bool EmailVerified { get; set; } = true;

        [JsonPropertyName("credentials")]
        public List<UserCredentials> Credentials { get; set; } = new();
    }

    public class UserCredentials
    {
        [JsonPropertyName("type")]
        public string Type { get; set; } = "password";

        [JsonPropertyName("temporary")]
        public bool Temporary { get; set; } = false;

        [JsonPropertyName("value")]
        public string Value { get; set; } = string.Empty;
    }
}