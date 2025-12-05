using System.Text.Json;
using MediatR;
using Microsoft.AspNetCore.Http.HttpResults;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.API.UseCases;
using WEB_353503_KOKHAN.Domain.Entities;

namespace WEB_353503_KOKHAN.API.EndPoints;

public static class CarEndpoints
{
    public static void MapCarEndpoints(this IEndpointRouteBuilder routes)
    {
        // Отключаем AntiForgery для работы с формами
        var group = routes.MapGroup("/api/Car")
            .WithTags(nameof(Car))
            .DisableAntiforgery();

        // Основной endpoint для получения списка автомобилей
        group.MapGet("/categories/{category:regex(^[a-z0-9\\-]+$)}",
            async (IMediator mediator, string? category, [FromQuery] int pageNo = 1, [FromQuery] int pageSize = 3) =>
            {
                var result = await mediator.Send(new GetListOfCars(category, pageNo, pageSize));
                return result.Successfull ? Results.Ok(result) : Results.BadRequest(result);
            })
            .WithName("GetCarsByCategory")
            .WithOpenApi()
            .AllowAnonymous();

        // Альтернативный endpoint без категории
        group.MapGet("/",
            async (IMediator mediator, [FromQuery] int pageNo = 1, [FromQuery] int pageSize = 3) =>
            {
                var result = await mediator.Send(new GetListOfCars(null, pageNo, pageSize));
                return result.Successfull ? Results.Ok(result) : Results.BadRequest(result);
            })
            .WithName("GetAllCars")
            .WithOpenApi()
            .AllowAnonymous();

        // Получение автомобиля по ID
        group.MapGet("/{id}", async Task<Results<Ok<Car>, NotFound>> (int id, AppDbContext db) =>
        {
            return await db.Cars.AsNoTracking()
                .Include(c => c.Category)
                .FirstOrDefaultAsync(model => model.Id == id)
                is Car model
                    ? TypedResults.Ok(model)
                    : TypedResults.NotFound();
        })
        .WithName("GetCarById")
        .WithOpenApi()
        .AllowAnonymous();


        group.MapPut("/{id}", async (
            int id,
            Car car,
            AppDbContext db,
            ILogger<Program> logger) =>
        {
            try
            {
                logger.LogInformation($"Simple update car {id}");

                var existingCar = await db.Cars.FindAsync(id);
                if (existingCar == null) return Results.NotFound();

                var currentImagePath = existingCar.ImagePath;
                var currentImageMimeType = existingCar.ImageMimeType;

                existingCar.Name = car.Name;
                existingCar.Description = car.Description;
                existingCar.CategoryId = car.CategoryId;
                existingCar.Price = car.Price;

                existingCar.ImagePath = currentImagePath;
                existingCar.ImageMimeType = currentImageMimeType;

                await db.SaveChangesAsync();
                return Results.Ok(existingCar);
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "Error in simple update");
                return Results.Problem($"Error: {ex.Message}");
            }
        })
        .WithName("UpdateCar")
        .WithOpenApi()
        .RequireAuthorization("admin");

        group.MapPost("/", async (
        [FromForm] string car,
        [FromForm] IFormFile? file,
        AppDbContext db,
        IMediator mediator,
        ILogger<Program> logger) =>
        {
            try
            {
                logger.LogInformation("=== CAR CREATION START ===");
                logger.LogInformation($"Raw car JSON from form: {car}");

                // Вариант 1: Десериализация с настройками
                var options = new JsonSerializerOptions
                {
                    PropertyNameCaseInsensitive = true,
                    PropertyNamingPolicy = JsonNamingPolicy.CamelCase
                };

                var newCar = JsonSerializer.Deserialize<Car>(car, options);

                logger.LogInformation("=== AFTER DESERIALIZATION ===");
                logger.LogInformation($"Car object: {JsonSerializer.Serialize(newCar, options)}");
                logger.LogInformation($"Name: '{newCar?.Name}' (is null: {newCar?.Name == null})");
                logger.LogInformation($"Name length: {newCar?.Name?.Length}");
                logger.LogInformation($"CategoryId: {newCar?.CategoryId}");

                if (newCar == null)
                {
                    logger.LogError("Failed to deserialize car JSON");
                    return Results.BadRequest("Invalid car data"); 
                }

                if (string.IsNullOrEmpty(newCar.Name))
                {
                    logger.LogError("Name is null or empty after deserialization!");
                    logger.LogError($"Full object: {JsonSerializer.Serialize(newCar)}");
                    return Results.BadRequest("Name is required"); 
                }

                // Обработка файла изображения
                if (file != null)
                {
                    logger.LogInformation("Processing image file");
                    newCar.ImagePath = await mediator.Send(new SaveImage(file));
                    newCar.ImageMimeType = file.ContentType;
                    logger.LogInformation($"Image saved: {newCar.ImagePath}");
                }
                else
                {
                    logger.LogInformation("No image file, using default");
                    newCar.ImagePath = "Images/noimage.jpg";
                    newCar.ImageMimeType = "image/jpeg";
                }

                logger.LogInformation("Adding car to database");
                db.Cars.Add(newCar);
                await db.SaveChangesAsync();

                logger.LogInformation($"Car created successfully with ID: {newCar.Id}");
                return Results.Created($"/api/Car/{newCar.Id}", newCar);
            }
            catch (JsonException jsonEx)
            {
                logger.LogError(jsonEx, "JSON deserialization error");
                return Results.BadRequest($"JSON Error: {jsonEx.Message}");
            }
            catch (DbUpdateException dbEx)
            {
                logger.LogError(dbEx, "Database error");
                return Results.BadRequest($"Database Error: {dbEx.Message}"); 
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "Unexpected error in car creation");
                return Results.Problem($"Internal Server Error: {ex.Message}"); 
            }
        })
        .WithName("CreateCar")
        .WithOpenApi()
        .RequireAuthorization("admin");

        group.MapPost("/{id}/update-with-image", async (
            int id,
            [FromForm] string car,
            [FromForm] IFormFile? file,
            AppDbContext db,
            IMediator mediator,
            ILogger<Program> logger) =>
        {
            try
            {
                logger.LogInformation($"Update car {id} with image");

                var existingCar = await db.Cars.FindAsync(id);
                if (existingCar == null)
                {
                    return Results.NotFound();
                }

                // Десериализация обновленных данных
                var options = new JsonSerializerOptions
                {
                    PropertyNameCaseInsensitive = true,
                    PropertyNamingPolicy = JsonNamingPolicy.CamelCase
                };

                var updatedCar = JsonSerializer.Deserialize<Car>(car, options);
                if (updatedCar == null)
                {
                    return Results.BadRequest("Invalid car data");
                }

                logger.LogInformation($"Updating car: Name='{updatedCar.Name}', CategoryId={updatedCar.CategoryId}");

                // Сохраняем старый путь к изображению для возможного удаления
                var oldImagePath = existingCar.ImagePath;

                // Обработка нового файла изображения
                if (file != null)
                {
                    try
                    {
                        // Сохраняем новое изображение
                        existingCar.ImagePath = await mediator.Send(new SaveImage(file));
                        existingCar.ImageMimeType = file.ContentType;

                        // Удаляем старое изображение (если это не дефолтное)
                        if (!string.IsNullOrEmpty(oldImagePath) && !oldImagePath.Contains("noimage.jpg"))
                        {
                            await mediator.Send(new DeleteImage(oldImagePath));
                        }
                    }
                    catch (Exception ex)
                    {
                        return Results.BadRequest($"Error handling image: {ex.Message}");
                    }
                }

                // Обновляем остальные поля
                existingCar.Name = updatedCar.Name;
                existingCar.Description = updatedCar.Description;
                existingCar.CategoryId = updatedCar.CategoryId;
                existingCar.Price = updatedCar.Price;

                await db.SaveChangesAsync();
                return Results.Ok(existingCar);
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "Error updating car with image");
                return Results.Problem($"Error: {ex.Message}");
            }
        })
        .WithName("UpdateCarWithImage")
        .Accepts<IFormFile>("multipart/form-data")
        .WithOpenApi()
        .RequireAuthorization("admin");

        // УДАЛЕНИЕ автомобиля с удалением изображения
        group.MapDelete("/{id}", async Task<Results<Ok, NotFound>> (
            int id,
            AppDbContext db,
            IMediator mediator) =>
        {
            var car = await db.Cars.FindAsync(id);
            if (car == null)
            {
                return TypedResults.NotFound();
            }

            // Удаляем изображение (если это не дефолтное)
            if (!car.ImagePath.Contains("noimage.jpg"))
            {
                await mediator.Send(new DeleteImage(car.ImagePath));
            }

            var affected = await db.Cars
                .Where(model => model.Id == id)
                .ExecuteDeleteAsync();

            return affected == 1 ? TypedResults.Ok() : TypedResults.NotFound();
        })
        .WithName("DeleteCar")
        .WithOpenApi()
        .RequireAuthorization("admin");

    }
}