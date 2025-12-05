using Microsoft.AspNetCore.Components.WebAssembly.Authentication;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.WebUtilities;
using System.Net.Http.Json;
using System.Text;
using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;

namespace WEB_353503_KOKHAN.BlazorWasm.Services
{
    public class DataService : IDataService
    {
        private readonly HttpClient _httpClient;
        private readonly IConfiguration _configuration;
        private readonly IAccessTokenProvider _tokenProvider;
        private readonly string _baseApiUrl;
        private readonly int _defaultPageSize;

        public DataService(HttpClient httpClient, IConfiguration configuration, IAccessTokenProvider tokenProvider)
        {
            _httpClient = httpClient;
            _configuration = configuration;
            _tokenProvider = tokenProvider;

            _baseApiUrl = _configuration["ApiSettings:BaseApiUrl"] ?? "https://localhost:7002/api/";

            if (int.TryParse(_configuration["ApiSettings:PageSize"], out var pageSize))
            {
                _defaultPageSize = pageSize;
            }
            else
            {
                _defaultPageSize = 3;
            }
        }

        public event Action DataLoaded = delegate { };

        public List<Category> Categories { get; set; } = new List<Category>();
        public List<Car> Cars { get; set; } = new List<Car>();
        public bool Success { get; set; }
        public string ErrorMessage { get; set; } = string.Empty;
        public int TotalPages { get; set; }
        public int CurrentPage { get; set; } = 1;
        public Category? SelectedCategory { get; set; } = null;

        private async Task<string> GetJwtTokenAsync()
        {
            var tokenResult = await _tokenProvider.RequestAccessToken();
            if (tokenResult.TryGetToken(out var token))
            {
                return token.Value;
            }
            throw new Exception("Не удалось получить токен");
        }

        public async Task GetProductListAsync(int pageNo = 1)
        {
            try
            {
                string url;

                if (SelectedCategory is not null)
                {
                    // Для категории: /api/Car/categories/sedan-cars/?pageNo=1&pageSize=3
                    var route = new StringBuilder("Car/categories/");
                    route.Append($"{SelectedCategory.NormalizedName}/");

                    List<KeyValuePair<string, string>> queryData = new();
                    queryData.Add(KeyValuePair.Create("pageNo", pageNo.ToString()));
                    queryData.Add(KeyValuePair.Create("pageSize", _defaultPageSize.ToString()));

                    var queryString = QueryString.Create(queryData);
                    url = $"{_baseApiUrl}{route}{queryString}";
                }
                else
                {
                    // Без категории: /api/Car/?pageNo=1&pageSize=3
                    url = $"{_baseApiUrl}Car/?pageNo={pageNo}&pageSize={_defaultPageSize}";
                }

                Console.WriteLine($"Request URL: {url}");

                var token = await GetJwtTokenAsync();
                var request = new HttpRequestMessage(HttpMethod.Get, url);
                request.Headers.Authorization = new System.Net.Http.Headers.AuthenticationHeaderValue("Bearer", token);

                var response = await _httpClient.SendAsync(request);
                var responseContent = await response.Content.ReadAsStringAsync();
                Console.WriteLine($"Response: {responseContent}");

                if (response.IsSuccessStatusCode)
                {
                    // Используем модели из Domain.Models для совместимости с API
                    var responseData = await response.Content.ReadFromJsonAsync<ResponseData<ListModel<Car>>>();

                    if (responseData != null && responseData.Successfull && responseData.Data?.Items != null)
                    {
                        Cars = responseData.Data.Items;
                        CurrentPage = responseData.Data.CurrentPage;
                        TotalPages = responseData.Data.TotalPages;
                        Success = true;
                        ErrorMessage = string.Empty;
                    }
                    else
                    {
                        Success = false;
                        ErrorMessage = responseData?.ErrorMessage ?? "Данные не получены";
                    }
                }
                else
                {
                    Success = false;
                    ErrorMessage = $"Ошибка HTTP: {response.StatusCode} - {responseContent}";
                }
            }
            catch (Exception ex)
            {
                Success = false;
                ErrorMessage = $"Исключение: {ex.Message}";
            }

            DataLoaded?.Invoke();
        }

        public async Task GetCategoryListAsync()
        {
            try
            {
                var url = $"{_baseApiUrl}Categories";
                var token = await GetJwtTokenAsync();

                var request = new HttpRequestMessage(HttpMethod.Get, url);
                request.Headers.Authorization = new System.Net.Http.Headers.AuthenticationHeaderValue("Bearer", token);

                var response = await _httpClient.SendAsync(request);
                var responseContent = await response.Content.ReadAsStringAsync();
                Console.WriteLine($"Categories Response: {responseContent}");

                if (response.IsSuccessStatusCode)
                {
                    // Используем модели из Domain.Models для совместимости с API
                    var categoryResponse = await response.Content.ReadFromJsonAsync<ResponseData<List<Category>>>();

                    if (categoryResponse != null && categoryResponse.Successfull && categoryResponse.Data != null)
                    {
                        Categories = categoryResponse.Data;
                        Success = true;
                        ErrorMessage = string.Empty;
                    }
                    else
                    {
                        Success = false;
                        ErrorMessage = categoryResponse?.ErrorMessage ?? "Категории не получены";
                    }
                }
                else
                {
                    Success = false;
                    ErrorMessage = $"Ошибка HTTP: {response.StatusCode} - {responseContent}";
                }
            }
            catch (Exception ex)
            {
                Success = false;
                ErrorMessage = ex.Message;
            }

            DataLoaded?.Invoke();
        }
    }
}