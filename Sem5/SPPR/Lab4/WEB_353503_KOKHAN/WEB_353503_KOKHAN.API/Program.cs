using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.API.EndPoints;
using WEB_353503_KOKHAN.API.Services.CarService;
using WEB_353503_KOKHAN.API.Services.CategoryService;
using MediatR;

namespace WEB_353503_KOKHAN.API
{
    public class Program
    {
        public static async Task Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            var connectionString = builder.Configuration.GetConnectionString("Sqlite");

            builder.Services.AddDbContext<AppDbContext>(options =>
                options.UseSqlite(connectionString));

            builder.Services.AddMediatR(cfg =>
            {
                cfg.RegisterServicesFromAssembly(typeof(Program).Assembly);
            });

            // Add services to the container.
            builder.Services.AddControllers();
            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            // Добавляем сервисы
            builder.Services.AddScoped<ICategoryService, CategoryService>();
            builder.Services.AddScoped<ICarService, CarService>();

            var app = builder.Build();

            // Инициализация базы данных
            using (var scope = app.Services.CreateScope())
            {
                var services = scope.ServiceProvider;

                await DbInitializer.SeedData(app);
                //try
                //{
                //    await DbInitializer.SeedData(app);
                //}
                //catch (Exception ex)
                //{
                //    var logger = services.GetRequiredService<ILogger<Program>>();
                //    logger.LogError(ex, "Ошибка при инициализации базы данных.");
                //}
            }

            // Configure the HTTP request pipeline.
            if (app.Environment.IsDevelopment())
            {
                app.UseSwagger();
                app.UseSwaggerUI();
            }

            app.UseStaticFiles();
            app.UseHttpsRedirection();
            app.UseAuthorization();

            app.MapControllers();
            app.MapCarEndpoints();

            await app.RunAsync();
        }
    }
}