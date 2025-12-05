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
            _logger.LogInformation($"-----> Запрос к API: {_httpClient.BaseAddress}{id}");

            var response = await _httpClient.GetAsync($"{id}");

            _logger.LogInformation($"-----> Ответ API: {response.StatusCode}");

            if (response.IsSuccessStatusCode)
            {
                // Сначала прочитайте как строку чтобы посмотреть что приходит
                var jsonString = await response.Content.ReadAsStringAsync();
                _logger.LogInformation($"-----> Сырой JSON ответ: {jsonString}");

                try
                {
                    // API возвращает Car, а не ResponseData<Car>
                    var car = await response.Content.ReadFromJsonAsync<Car>(_serializerOptions);
                    _logger.LogInformation($"-----> Десериализация Car успешна: {car != null}");

                    // Оберните в ResponseData
                    return ResponseData<Car>.Success(car);
                }
                catch (Exception ex)
                {
                    _logger.LogError($"-----> Ошибка десериализации: {ex.Message}");
                    return ResponseData<Car>.Error($"Ошибка десериализации: {ex.Message}");
                }
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
        using var form = new MultipartFormDataContent();

        var carJson = JsonSerializer.Serialize(car, _serializerOptions);
        form.Add(new StringContent(carJson, Encoding.UTF8, "application/json"), "car");

        if (formFile != null)
        {
            var streamContent = new StreamContent(formFile.OpenReadStream());
            streamContent.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue(formFile.ContentType);
            form.Add(streamContent, "file", formFile.FileName);
        }

        var endpoint = formFile != null ? $"{id}/update-with-image" : $"{id}";

        HttpResponseMessage response;
        if (formFile != null)
        {
            response = await _httpClient.PostAsync(endpoint, form);
        }
        else
        {
            response = await _httpClient.PutAsJsonAsync(endpoint, car, _serializerOptions); 
        }

        if (!response.IsSuccessStatusCode)
        {
            var errorContent = await response.Content.ReadAsStringAsync();
            _logger.LogError($"Ошибка обновления: {response.StatusCode}, Content: {errorContent}");
            throw new HttpRequestException($"Ошибка обновления: {response.StatusCode} - {errorContent}");
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
        try
        {
            _logger.LogInformation($"Creating car: {JsonSerializer.Serialize(car)}");

            using var form = new MultipartFormDataContent();

            // Сериализуем объект Car в JSON
            var carJson = JsonSerializer.Serialize(car, _serializerOptions);
            _logger.LogInformation($"Car JSON: {carJson}");

            form.Add(new StringContent(carJson, Encoding.UTF8, "application/json"), "car");

            if (formFile != null)
            {
                _logger.LogInformation($"Adding file: {formFile.FileName}, Size: {formFile.Length}");
                var streamContent = new StreamContent(formFile.OpenReadStream());
                streamContent.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue(formFile.ContentType);
                form.Add(streamContent, "file", formFile.FileName);
            }

            var response = await _httpClient.PostAsync("", form);

            if (response.IsSuccessStatusCode)
            {
                var responseData = await response.Content.ReadFromJsonAsync<ResponseData<Car>>(_serializerOptions);
                return responseData ?? ResponseData<Car>.Error("Пустой ответ при создании");
            }

            // Читаем подробную ошибку от API
            var errorContent = await response.Content.ReadAsStringAsync();
            _logger.LogError($"API Error: {response.StatusCode}, Content: {errorContent}");

            return ResponseData<Car>.Error($"Ошибка: {response.StatusCode} - {errorContent}");
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, $"Ошибка при создании объекта");
            return ResponseData<Car>.Error($"Ошибка: {ex.Message}");
        }
    }
}