using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.Domain.Entities;

namespace WEB_353503_KOKHAN.API.Data
{
    public static class DbInitializer
    {
        public static async Task SeedData(WebApplication app)
        {
            using var scope = app.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<AppDbContext>();

            // Выполнение миграций
            await context.Database.MigrateAsync();

            var baseUrl = app.Configuration.GetValue<string>("AppSettings:BaseUrl");

            if (context.Categories.Any() || context.Cars.Any())
            {
                return;
            }

            // Создаем категории (БЕЗ указания Id!)
            var categories = new List<Category>
            {
                new Category { Name = "Седан", NormalizedName = "sedan-cars" },
                new Category { Name = "Хэтчбек", NormalizedName = "hatchback-cars" },
                new Category { Name = "Универсал", NormalizedName = "station-wagon-cars" },
                new Category { Name = "Кроссовер", NormalizedName = "cuv-cars" },
                new Category { Name = "Внедорожник", NormalizedName = "suv-cars" },
                new Category { Name = "Купе", NormalizedName = "coupe-cars" },
                new Category { Name = "Кабриолет", NormalizedName = "cabriolet-cars" },
                new Category { Name = "Минивэн", NormalizedName = "mpv-cars" },
                new Category { Name = "Лифтбек", NormalizedName = "liftback-cars" },
                new Category { Name = "Пикап", NormalizedName = "pick-up-cars" }
            };

            // Добавляем категории в контекст БД
            await context.Categories.AddRangeAsync(categories);
            await context.SaveChangesAsync(); // Сохраняем, чтобы получить сгенерированные Id

            // Создаем автомобили (БЕЗ указания Id и с правильными ссылками на категории!)
            var cars = new List<Car>
            {
                new Car
                {
                    Name = "Granta Sedan 2025",
                    Description = "Современный, технологичный, стремительный и стильный — автомобиль, каким он должен быть",
                    Price = 33900,
                    ImagePath = $"{baseUrl}/images/cars/granta_sedan.png",
                    ImageMimeType = "image/png",
                    Category = categories.First(c => c.NormalizedName == "sedan-cars")
                },
                new Car
                {
                    Name = "Granta Liftback 2025",
                    Description = "Динамичный стиль и обтекаемые формы признак автомобиля со спортивным характером. Но лифтбек Granta еще и потрясающе вместителен",
                    Price = 34900,
                    ImagePath = $"{baseUrl}/images/cars/granta_liftback.png",
                    ImageMimeType = "image/png",
                    Category = categories.First(c => c.NormalizedName == "liftback-cars")
                },
                new Car
                {
                    Name = "Vesta SW 2025",
                    Description = "Стильный, вместительный, комфортный универсал. Современное сочетание практичности и элегантности",
                    Price = 49500,
                    ImagePath = $"{baseUrl}/images/cars/vesta_station_wagon.png",
                    ImageMimeType = "image/png",
                    Category = categories.First(c => c.NormalizedName == "station-wagon-cars")
                },
                new Car
                {
                    Name = "Vesta Sedan 2025",
                    Description = "Вместимость и оснащенность, элегантность и практичность — Новая Vesta превосходна во всем",
                    Price = 47500,
                    ImagePath = $"{baseUrl}/images/cars/vesta_sedan.png",
                    ImageMimeType = "image/png",
                    Category = categories.First(c => c.NormalizedName == "sedan-cars")
                },
                new Car
                {
                    Name = "Largus 2025",
                    Description = "Путешественник, партнер по бизнесу и член семьи — это LADA Largus: комфортный, яркий и функциональный",
                    Price = 47000,
                    ImagePath = $"{baseUrl}/images/cars/largus_station_wagon.png",
                    ImageMimeType = "image/png",
                    Category = categories.First(c => c.NormalizedName == "station-wagon-cars")
                },
                new Car
                {
                    Name = "Niva Legend 2025",
                    Description = "LADA Niva Legend — узнаваемый стиль и прославленная проходимость",
                    Price = 34580,
                    ImagePath = $"{baseUrl}/images/cars/niva_suv.png",
                    ImageMimeType = "image/png",
                    Category = categories.First(c => c.NormalizedName == "suv-cars")
                },
                new Car
                {
                    Name = "Niva Travel 2025",
                    Description = "LADA Niva Travel — это мощь полного привода и постоянная готовность к приключениям. К приключениям с комфортом",
                    Price = 43050,
                    ImagePath = $"{baseUrl}/images/cars/niva_travel_suv.png",
                    ImageMimeType = "image/png",
                    Category = categories.First(c => c.NormalizedName == "suv-cars")
                }
            };

            await context.Cars.AddRangeAsync(cars);
            await context.SaveChangesAsync();

            Console.WriteLine("База данных успешно заполнена начальными данными!");
        }
    }
}