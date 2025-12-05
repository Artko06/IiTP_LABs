using System.Text;
using System.Text.Json;
using Microsoft.Extensions.Logging;
using WEB_353503_KOKHAN.Domain.Models;
using WEB_353503_KOKHAN.UI.Services.CarService;
using WEB_353503_KOKHAN.UI.Services.Authentication;

namespace WEB_353503_KOKHAN.UI.Services.CarService;

public class ApiCarService : ICarService
{
    private readonly HttpClient _httpClient;
    private readonly string _pageSize;
    private readonly JsonSerializerOptions _serializerOptions;
    private readonly ILogger<ApiCarService> _logger;
    private readonly ITokenAccessor _tokenAccessor;

    public ApiCarService(
        HttpClient httpClient,
        IConfiguration configuration,
        ILogger<ApiCarService> logger,
        ITokenAccessor tokenAccessor) // Добавляем ITokenAccessor
    {
        _httpClient = httpClient;
        _pageSize = configuration.GetSection("ItemsPerPage").Value ?? "3";
        _serializerOptions = new JsonSerializerOptions
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase
        };
        _logger = logger;
        _tokenAccessor = tokenAccessor;
    }

    public async Task<ResponseData<ListModel<Car>>> GetProductListAsync(
        string? categoryNormalizedName = null,
        int pageNo = 1)
    {
        try
        {
            // Для GET-запросов (чтение) обычно не требуется авторизация
            // Но если нужно, можно добавить:
            // await _tokenAccessor.SetAuthorizationHeaderAsync(_httpClient, false);

            string url;

            if (string.IsNullOrEmpty(categoryNormalizedName))
            {
                url = $"?pageNo={pageNo}&pageSize={_pageSize}";
            }
            else
            {
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
            // Для GET-запросов (чтение) обычно не требуется авторизация
            // await _tokenAccessor.SetAuthorizationHeaderAsync(_httpClient, false);

            _logger.LogInformation($"-----> Запрос к API: {_httpClient.BaseAddress}{id}");

            var response = await _httpClient.GetAsync($"{id}");

            _logger.LogInformation($"-----> Ответ API: {response.StatusCode}");

            if (response.IsSuccessStatusCode)
            {
                var jsonString = await response.Content.ReadAsStringAsync();
                _logger.LogInformation($"-----> Сырой JSON ответ: {jsonString}");

                try
                {
                    var car = await response.Content.ReadFromJsonAsync<Car>(_serializerOptions);
                    _logger.LogInformation($"-----> Десериализация Car успешна: {car != null}");

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
        try
        {
            // Для операций изменения добавляем авторизацию
            await _tokenAccessor.SetAuthorizationHeaderAsync(_httpClient, false);

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

                if (response.StatusCode == System.Net.HttpStatusCode.Unauthorized)
                {
                    throw new UnauthorizedAccessException("Требуется авторизация для выполнения операции");
                }

                throw new HttpRequestException($"Ошибка обновления: {response.StatusCode} - {errorContent}");
            }
        }
        catch (AuthenticationFailureException ex)
        {
            _logger.LogWarning(ex, "Ошибка аутентификации при обновлении автомобиля {Id}", id);
            throw new UnauthorizedAccessException("Требуется авторизация администратора", ex);
        }
        catch (Exception ex) when (ex is not UnauthorizedAccessException)
        {
            _logger.LogError(ex, "Ошибка при обновлении автомобиля {Id}", id);
            throw;
        }
    }

    public async Task DeleteProductAsync(int id)
    {
        try
        {
            // Для операций удаления добавляем авторизацию
            await _tokenAccessor.SetAuthorizationHeaderAsync(_httpClient, false);

            var response = await _httpClient.DeleteAsync($"{id}");

            if (!response.IsSuccessStatusCode)
            {
                var errorContent = await response.Content.ReadAsStringAsync();
                _logger.LogError($"-----> Объект не удалён. Ошибка: {response.StatusCode}");

                if (response.StatusCode == System.Net.HttpStatusCode.Unauthorized)
                {
                    throw new UnauthorizedAccessException("Требуется авторизация для выполнения операции");
                }

                throw new HttpRequestException($"Ошибка удаления: {response.StatusCode} - {errorContent}");
            }
        }
        catch (AuthenticationFailureException ex)
        {
            _logger.LogWarning(ex, "Ошибка аутентификации при удалении автомобиля {Id}", id);
            throw new UnauthorizedAccessException("Требуется авторизация администратора", ex);
        }
        catch (Exception ex) when (ex is not UnauthorizedAccessException)
        {
            _logger.LogError(ex, "Ошибка при удалении автомобиля {Id}", id);
            throw;
        }
    }

    public async Task<ResponseData<Car>> CreateProductAsync(Car car, IFormFile? formFile)
    {
        try
        {
            // Для операций создания добавляем авторизацию
            await _tokenAccessor.SetAuthorizationHeaderAsync(_httpClient, false);

            _logger.LogInformation($"Creating car: {JsonSerializer.Serialize(car)}");

            using var form = new MultipartFormDataContent();

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

            var errorContent = await response.Content.ReadAsStringAsync();
            _logger.LogError($"API Error: {response.StatusCode}, Content: {errorContent}");

            if (response.StatusCode == System.Net.HttpStatusCode.Unauthorized)
            {
                return ResponseData<Car>.Error("Требуется авторизация для создания объекта");
            }

            return ResponseData<Car>.Error($"Ошибка: {response.StatusCode} - {errorContent}");
        }
        catch (AuthenticationFailureException ex)
        {
            _logger.LogWarning(ex, "Ошибка аутентификации при создании автомобиля");
            return ResponseData<Car>.Error("Требуется авторизация администратора");
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, $"Ошибка при создании объекта");
            return ResponseData<Car>.Error($"Ошибка: {ex.Message}");
        }
    }
}