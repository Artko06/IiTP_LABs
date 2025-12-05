using System.Text;
using System.Text.Json;
using Microsoft.Extensions.Logging;
using WEB_353503_KOKHAN.Domain.Models;
using WEB_353503_KOKHAN.UI.Services.CarService;

namespace WEB_353503_KOKHAN.UI.Services.CarService;

public class ApiCarService : ICarService
{
    private readonly HttpClient _httpClient;
    private readonly string _pageSize;
    private readonly JsonSerializerOptions _serializerOptions;
    private readonly ILogger<ApiCarService> _logger;

    public ApiCarService(
        HttpClient httpClient,
        IConfiguration configuration,
        ILogger<ApiCarService> logger)
    {
        _httpClient = httpClient;
        _pageSize = configuration.GetSection("ItemsPerPage").Value ?? "3";
        _serializerOptions = new JsonSerializerOptions
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase
        };
        _logger = logger;
    }

    public async Task<ResponseData<ListModel<Car>>> GetProductListAsync(
    string? categoryNormalizedName = null,
    int pageNo = 1)
    {
        try
        {
            string url;

            if (string.IsNullOrEmpty(categoryNormalizedName))
            {
                // Без категории: /api/Car?pageNo=1&pageSize=3
                url = $"?pageNo={pageNo}&pageSize={_pageSize}";
            }
            else
            {
                // С категорией: /api/Car/categories/sedan-cars?pageNo=1&pageSize=3
                url = $"categories/{categoryNormalizedName}?pageNo={pageNo}&pageSize={_pageSize}";
            }

            _logger.LogInformation($"-----> Запрос к API: {_httpClient.BaseAddress}{url}");

            var response = await _httpClient.GetAsync(url);

            if (response.IsSuccessStatusCode)
            {
                var result = await response.Content.ReadFromJsonAsync<ResponseData<ListModel<Car>>>(_serializerOptions);
                return result ?? ResponseData<ListModel<Car>>.Error("Получен пустой ответ от сервера");
            }

            _logger.LogError($"-----> Данные не получены от сервера. Ошибка: {response.StatusCode}");
            return ResponseData<ListModel<Car>>.Error($"Данные не получены от сервера. Ошибка: {response.StatusCode}");
        }
        catch (Exception ex)
        {
            _logger.LogError($"-----> Ошибка: {ex.Message}");
            return ResponseData<ListModel<Car>>.Error($"Ошибка: {ex.Message}");
        }
    }

    public async Task<ResponseData<Car>> GetProductByIdAsync(int id)
    {
        try
        {
            var response = await _httpClient.GetAsync($"{id}");
            if (response.IsSuccessStatusCode)
            {
                var result = await response.Content.ReadFromJsonAsync<ResponseData<Car>>(_serializerOptions);
                return result ?? ResponseData<Car>.Error("Объект не найден");
            }

            _logger.LogError($"-----> Объект не найден. Ошибка: {response.StatusCode}");
            return ResponseData<Car>.Error($"Ошибка: {response.StatusCode}");
        }
        catch (Exception ex)
        {
            _logger.LogError($"-----> Ошибка при получении объекта: {ex.Message}");
            return ResponseData<Car>.Error($"Ошибка: {ex.Message}");
        }
    }

    public async Task UpdateProductAsync(int id, Car car, IFormFile? formFile)
    {
        HttpResponseMessage response;

        if (formFile != null)
        {
            using var form = new MultipartFormDataContent();
            using var fileStream = formFile.OpenReadStream();
            using var fileContent = new StreamContent(fileStream);
            fileContent.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue(formFile.ContentType);
            form.Add(fileContent, "file", formFile.FileName);
            form.Add(new StringContent(car.Name), "name");
            form.Add(new StringContent(car.Description), "description");
            form.Add(new StringContent(car.Price.ToString()), "price");
            form.Add(new StringContent(car.CategoryId.ToString()), "categoryId");

            response = await _httpClient.PutAsync($"{id}", form);
        }
        else
        {
            response = await _httpClient.PutAsJsonAsync($"{id}", car, _serializerOptions);
        }

        if (!response.IsSuccessStatusCode)
        {
            _logger.LogError($"-----> Объект не обновлён. Ошибка: {response.StatusCode}");
            throw new HttpRequestException($"Ошибка обновления: {response.ReasonPhrase}");
        }
    }

    public async Task DeleteProductAsync(int id)
    {
        var response = await _httpClient.DeleteAsync($"{id}");
        if (!response.IsSuccessStatusCode)
        {
            _logger.LogError($"-----> Объект не удалён. Ошибка: {response.StatusCode}");
            throw new HttpRequestException($"Ошибка удаления: {response.ReasonPhrase}");
        }
    }

    public async Task<ResponseData<Car>> CreateProductAsync(Car car, IFormFile? formFile)
    {
        car.ImagePath = "Images/noimage.jpg";

        var request = new HttpRequestMessage
        {
            Method = HttpMethod.Post,
            RequestUri = _httpClient.BaseAddress
        };

        if (formFile != null)
        {
            using var form = new MultipartFormDataContent();
            using var fileStream = formFile.OpenReadStream();
            using var fileContent = new StreamContent(fileStream);
            fileContent.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue(formFile.ContentType);
            form.Add(fileContent, "file", formFile.FileName);
            form.Add(new StringContent(car.Name), "name");
            form.Add(new StringContent(car.Description), "description");
            form.Add(new StringContent(car.Price.ToString()), "price");
            form.Add(new StringContent(car.CategoryId.ToString()), "categoryId");

            request.Content = form;
        }
        else
        {
            var json = JsonSerializer.Serialize(car, _serializerOptions);
            request.Content = new StringContent(json, Encoding.UTF8, "application/json");
        }

        try
        {
            var response = await _httpClient.SendAsync(request, CancellationToken.None);
            if (response.IsSuccessStatusCode)
            {
                var responseData = await response.Content.ReadFromJsonAsync<ResponseData<Car>>(_serializerOptions);
                return responseData ?? ResponseData<Car>.Error("Пустой ответ при создании");
            }

            _logger.LogError($"-----> Объект не создан. Ошибка: {response.StatusCode}");
            return ResponseData<Car>.Error($"Ошибка: {response.StatusCode}");
        }
        catch (Exception ex)
        {
            _logger.LogError($"-----> Ошибка при создании объекта: {ex.Message}");
            return ResponseData<Car>.Error($"Ошибка: {ex.Message}");
        }
    }
}