using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.IdentityModel.Protocols.OpenIdConnect;
using WEB_353503_KOKHAN.UI.Controllers;
using WEB_353503_KOKHAN.UI.Extensions;
using WEB_353503_KOKHAN.UI.HelperClasses;
using WEB_353503_KOKHAN.UI.Models;
using WEB_353503_KOKHAN.UI.Services.Authentication;
using WEB_353503_KOKHAN.UI.Services.FileService;

namespace WEB_353503_KOKHAN.UI
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            var keycloakData = builder.Configuration.GetSection("Keycloak").Get<KeycloakData>();
            if (keycloakData == null)
            {
                throw new InvalidOperationException("Keycloak configuration is not set.");
            }

            // HttpContextAccessor
            builder.Services.AddHttpContextAccessor();

            builder.Services.AddScoped<IFileService, LocalFileService>();
            builder.Services.AddHttpClient<AccountController>();

            // HttpClient дл€ TokenAccessor
            builder.Services.AddHttpClient<ITokenAccessor, KeycloakTokenAccessor>();

            builder.Services
                .AddAuthentication(options =>
                {
                    options.DefaultScheme = CookieAuthenticationDefaults.AuthenticationScheme;
                    options.DefaultChallengeScheme = "keycloak";
                })
                .AddCookie()
                .AddOpenIdConnect("keycloak", options =>
                {
                    // Authority с /auth/ перед /realms
                    options.Authority = $"{keycloakData.Host}/auth/realms/{keycloakData.Realm}";
                    options.ClientId = keycloakData.ClientId;
                    options.ClientSecret = keycloakData.ClientSecret;
                    options.ResponseType = OpenIdConnectResponseType.Code;

                    // явное указание Scope
                    options.Scope.Clear();
                    options.Scope.Add("openid");

                    options.SaveTokens = true;
                    options.RequireHttpsMetadata = false;

                    // явное указание MetadataAddress (важно!)
                    options.MetadataAddress =
                        $"{keycloakData.Host}/realms/{keycloakData.Realm}/.well-known/openid-configuration";
                });

            builder.Services.AddAuthorization(options =>
            {
                options.AddPolicy("admin", policy =>
                {
                    policy.RequireClaim("http://schemas.microsoft.com/ws/2008/06/identity/claims/role", "POWER_USER");
                });
            });

            builder.Services.Configure<UriData>(builder.Configuration.GetSection("UriData"));

            var uriData = builder.Configuration.GetSection("UriData").Get<UriData>()
                ?? throw new InvalidOperationException("Section 'UriData' not found in configuration.");

            builder.RegisterCustomServices();

            builder.Services.AddHttpClient<ICarService, ApiCarService>(client =>
            {
                client.BaseAddress = new Uri(new Uri(uriData.ApiUri), "Car/");
            });

            builder.Services.AddHttpClient<ICategoryService, ApiCategoryService>(client =>
            {
                client.BaseAddress = new Uri(uriData.ApiUri);
            });


            // Add services to the container.
            builder.Services.AddRazorPages();

            builder.Services.AddControllersWithViews();

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (!app.Environment.IsDevelopment())
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }

            app.UseHttpsRedirection();
            app.UseStaticFiles();

            app.UseRouting();

            app.UseAuthorization();

            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=Index}/{id?}"
                );

            app.MapRazorPages()
                .RequireAuthorization("admin");

            app.Run();
        }
    }
}
