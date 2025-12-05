using MediatR;
using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;

namespace WEB_353503_KOKHAN.API.UseCases
{
    public sealed record GetListOfCars(
        string? categoryNormalizedName,
        int pageNo = 1,
        int pageSize = 3) : IRequest<ResponseData<ListModel<Car>>>;

    public class GetListOfCarsHandler : IRequestHandler<GetListOfCars, ResponseData<ListModel<Car>>>
    {
        private readonly AppDbContext _db;
        private readonly int _maxPageSize = 20;

        public GetListOfCarsHandler(AppDbContext db)
        {
            _db = db;
        }

        public async Task<ResponseData<ListModel<Car>>> Handle(GetListOfCars request, CancellationToken cancellationToken)
        {
            try
            {
                var query = _db.Cars.AsQueryable();

                // Фильтрация по категории, если указана
                if (!string.IsNullOrEmpty(request.categoryNormalizedName))
                {
                    query = query.Where(c => c.Category != null &&
                                           c.Category.NormalizedName == request.categoryNormalizedName);
                }

                // Валидация размера страницы
                var pageSize = Math.Min(request.pageSize, _maxPageSize);
                if (pageSize < 1) pageSize = 3;

                // Получение общего количества элементов
                var totalCount = await query.CountAsync(cancellationToken);

                // Вычисление количества страниц
                var totalPages = totalCount > 0 ? (int)Math.Ceiling(totalCount / (double)pageSize) : 1;

                // Валидация номера страницы
                var pageNo = Math.Max(1, Math.Min(request.pageNo, totalPages));

                // Получение данных для текущей страницы
                var items = await query
                    .Skip((pageNo - 1) * pageSize)
                    .Take(pageSize)
                    .Include(c => c.Category) // Включаем связанные данные категории
                    .ToListAsync(cancellationToken);

                // Создание модели списка согласно вашей структуре
                var listModel = new ListModel<Car>
                {
                    Items = items,
                    CurrentPage = pageNo,
                    TotalPages = totalPages
                };

                // Возвращаем успешный ответ
                return ResponseData<ListModel<Car>>.Success(listModel);
            }
            catch (Exception ex)
            {
                // Возвращаем ошибку в случае исключения
                return ResponseData<ListModel<Car>>.Error($"Ошибка при получении списка автомобилей: {ex.Message}");
            }
        }
    }
}