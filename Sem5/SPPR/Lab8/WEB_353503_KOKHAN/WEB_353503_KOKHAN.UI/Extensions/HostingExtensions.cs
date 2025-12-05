using WEB_353503_KOKHAN.UI.HelperClasses;

namespace WEB_353503_KOKHAN.UI.Extensions
{
    public static class HostingExtensions
    {
        public static void RegisterCustomServices(this WebApplicationBuilder builder)
        {
            builder.Services.AddHttpClient<ICarService, ApiCarService>(client =>
            {
                client.BaseAddress = new Uri(builder.Configuration["UriData:ApiUri"]);
            });

            builder.Services.AddHttpClient<ICarService, ApiCarService>(client =>
            {
                client.BaseAddress = new Uri(builder.Configuration["UriData:ApiUri"]);
            });

            builder.Services.Configure<KeycloakData>(builder.Configuration.GetSection("Keycloak"));
        }
    }
}
