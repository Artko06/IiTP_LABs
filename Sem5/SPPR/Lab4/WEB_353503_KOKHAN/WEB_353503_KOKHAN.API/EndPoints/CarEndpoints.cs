using MediatR;
using Microsoft.AspNetCore.Http.HttpResults;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.OpenApi;
using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.API.UseCases;
using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;

namespace WEB_353503_KOKHAN.API.EndPoints;

public static class CarEndpoints
{
    public static void MapCarEndpoints(this IEndpointRouteBuilder routes)
    {
        var group = routes.MapGroup("/api/Car").WithTags(nameof(Car));

        // Основной endpoint для получения списка автомобилей с пагинацией и фильтрацией
        group.MapGet("/categories/{category:regex(^[a-z0-9\\-]+$)}",
            async (IMediator mediator, string? category, [FromQuery] int pageNo = 1, [FromQuery] int pageSize = 3) =>
            {
                var result = await mediator.Send(new GetListOfCars(category, pageNo, pageSize));
                return result.Successfull ? Results.Ok(result) : Results.BadRequest(result);
            })
            .WithName("GetCarsByCategory")
            .WithOpenApi();

        // Альтернативный endpoint без категории
        group.MapGet("/",
            async (IMediator mediator, int pageNo = 1, int pageSize = 3) =>
            {
                var result = await mediator.Send(new GetListOfCars(null, pageNo, pageSize));

                if (!result.Successfull)
                {
                    return Results.BadRequest(result);
                }

                return Results.Ok(result);
            })
            .WithName("GetAllCars")
            .WithOpenApi();

        // Остальные endpoints остаются без изменений
        group.MapGet("/{id}", async Task<Results<Ok<Car>, NotFound>> (int id, AppDbContext db) =>
        {
            return await db.Cars.AsNoTracking()
                .FirstOrDefaultAsync(model => model.Id == id)
                is Car model
                    ? TypedResults.Ok(model)
                    : TypedResults.NotFound();
        })
        .WithName("GetCarById")
        .WithOpenApi();

        group.MapPut("/{id}", async Task<Results<Ok, NotFound>> (int id, Car car, AppDbContext db) =>
        {
            var affected = await db.Cars
                .Where(model => model.Id == id)
                .ExecuteUpdateAsync(setters => setters
                    .SetProperty(m => m.Id, car.Id)
                    .SetProperty(m => m.Name, car.Name)
                    .SetProperty(m => m.Description, car.Description)
                    .SetProperty(m => m.CategoryId, car.CategoryId)
                    .SetProperty(m => m.Price, car.Price)
                    .SetProperty(m => m.ImagePath, car.ImagePath)
                    .SetProperty(m => m.ImageMimeType, car.ImageMimeType)
                    );
            return affected == 1 ? TypedResults.Ok() : TypedResults.NotFound();
        })
        .WithName("UpdateCar")
        .WithOpenApi();

        group.MapPost("/", async (Car car, AppDbContext db) =>
        {
            db.Cars.Add(car);
            await db.SaveChangesAsync();
            return TypedResults.Created($"/api/Car/{car.Id}", car);
        })
        .WithName("CreateCar")
        .WithOpenApi();

        group.MapDelete("/{id}", async Task<Results<Ok, NotFound>> (int id, AppDbContext db) =>
        {
            var affected = await db.Cars
                .Where(model => model.Id == id)
                .ExecuteDeleteAsync();
            return affected == 1 ? TypedResults.Ok() : TypedResults.NotFound();
        })
        .WithName("DeleteCar")
        .WithOpenApi();
    }
}