using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.API.EndPoints;
using WEB_353503_KOKHAN.API.Services.CarService;
using WEB_353503_KOKHAN.API.Services.CategoryService;
using MediatR;
using System.Text.Json;
using Microsoft.AspNetCore.Http.Json;
using WEB_353503_KOKHAN.API.Models;
using Microsoft.AspNetCore.Authentication.JwtBearer;

namespace WEB_353503_KOKHAN.API
{
    public class Program
    {
        public static async Task Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            builder.Services.AddHybridCache();

            builder.Services.AddStackExchangeRedisCache(opt =>
            {
                opt.InstanceName = "labs_";
                opt.Configuration = builder.Configuration.GetConnectionString("Redis");
            });

            var authServer = builder.Configuration
                .GetSection("AuthServer")
                .Get<AuthServerData>();

            builder.Services.AddAuthentication(JwtBearerDefaults.AuthenticationScheme)
                .AddJwtBearer(JwtBearerDefaults.AuthenticationScheme, o =>
                {
                    o.MetadataAddress = $"{authServer.Host}/realms/{authServer.Realm}/.well-known/openid-configuration";

                    o.Authority = $"{authServer.Host}/realms/{authServer.Realm}";

                    o.Audience = "account";

                    o.RequireHttpsMetadata = false;
                });

            builder.Services.AddAuthorization(options =>
            {
                options.AddPolicy("admin", policy =>
                {
                    policy.RequireClaim("http://schemas.microsoft.com/ws/2008/06/identity/claims/role", "POWER_USER");
                });
            });

            builder.Services.AddCors(options =>
            {
                options.AddPolicy("AllowBlazorWasm", builder =>
                {
                    builder.WithOrigins("https://localhost:7004")
                           .AllowAnyHeader()
                           .AllowAnyMethod()
                           .AllowCredentials();
                });
            });

            var connectionString = builder.Configuration.GetConnectionString("Sqlite");

            builder.Services.AddHttpContextAccessor();

            builder.Services.AddDbContext<AppDbContext>(options =>
                options.UseSqlite(connectionString));

            builder.Services.AddMediatR(cfg =>
            {
                cfg.RegisterServicesFromAssembly(typeof(Program).Assembly);
            });

            // Для работы со статическими файлами
            builder.Services.Configure<IISServerOptions>(options =>
            {
                options.AllowSynchronousIO = true;
            });

            builder.Services.Configure<JsonOptions>(options =>
            {
                options.SerializerOptions.PropertyNamingPolicy = JsonNamingPolicy.CamelCase;
            });

            builder.Services.AddControllers()
            .AddJsonOptions(options =>
            {
                options.JsonSerializerOptions.PropertyNamingPolicy = JsonNamingPolicy.CamelCase;
                options.JsonSerializerOptions.PropertyNameCaseInsensitive = true;
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

            app.UseCors("AllowBlazorWasm");

            app.UseStaticFiles();

            app.UseHttpsRedirection();

            app.UseAuthentication();
            app.UseAuthorization();

            app.MapControllers();
            app.MapCarEndpoints();

            await app.RunAsync();
        }
    }
}