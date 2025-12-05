using WEB_353503_KOKHAN.UI.Extensions;
using WEB_353503_KOKHAN.UI.Models;

namespace WEB_353503_KOKHAN.UI
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

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

            //app.MapControllerRoute(
            //    name: "areas",
            //    pattern: "{area:exists}/{controller=Home}/{action=Index}/{id?}"
            //    );

            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=Index}/{id?}"
                );

            app.MapRazorPages();

            app.Run();
        }
    }
}
