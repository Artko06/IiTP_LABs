using Microsoft.AspNetCore.Authentication;
using Microsoft.Extensions.Options;
using NuGet.Common;
using System;
using System.Net.Http.Headers;
using System.Text.Json;
using System.Text.Json.Nodes;
using WEB_353503_KOKHAN.UI.HelperClasses;

namespace WEB_353503_KOKHAN.UI.Services.Authentication
{ 
    public class KeycloakTokenAccessor : ITokenAccessor
    {
        private readonly IHttpContextAccessor _contextAccessor;
        private readonly KeycloakData _keycloakData;
        private readonly HttpClient _httpClient;

        public KeycloakTokenAccessor(
            IHttpContextAccessor contextAccessor,
            IOptions<KeycloakData> options,
            HttpClient httpClient)
        {
            _contextAccessor = contextAccessor;
            _keycloakData = options.Value;
            _httpClient = httpClient;
        }

        public async Task SetAuthorizationHeaderAsync(HttpClient httpClient, bool isClient)
        {
            string token = isClient
                ? await GetClientToken()
                : await GetUserToken();

            httpClient.DefaultRequestHeaders.Authorization =
                new AuthenticationHeaderValue("Bearer", token);
        }

        private async Task<string> GetUserToken()
        {
            var context = _contextAccessor.HttpContext;
            if (context == null)
            {
                throw new AuthenticationFailureException("HTTP контекст недоступен");
            }

            // Проверяем, аутентифицирован ли пользователь
            if (!context.User.Identity.IsAuthenticated)
            {
                throw new AuthenticationFailureException("Пользователь неавторизован");
            }

            // Получаем токен из контекста
            var token = await context.GetTokenAsync("keycloak", "access_token");

            if (string.IsNullOrEmpty(token))
            {
                throw new AuthenticationFailureException("Токен пользователя не найден");
            }

            return token;
        }

        private async Task<string> GetClientToken()
        {
            // Keycloak token endpoint
            var requestUrl = $"{_keycloakData.Host}/realms/{_keycloakData.Realm}/protocol/openid-connect/token";

            // Http request content
            var content = new FormUrlEncodedContent(new[]
            {
            new KeyValuePair<string, string>("client_id", _keycloakData.ClientId),
            new KeyValuePair<string, string>("grant_type", "client_credentials"),
            new KeyValuePair<string, string>("client_secret", _keycloakData.ClientSecret)
        });

            // Send request
            var response = await _httpClient.PostAsync(requestUrl, content);

            if (!response.IsSuccessStatusCode)
            {
                var errorContent = await response.Content.ReadAsStringAsync();
                throw new HttpRequestException($"Ошибка получения токена клиента: {response.StatusCode}. {errorContent}");
            }

            // Extract access token from response
            var jsonString = await response.Content.ReadAsStringAsync();
            using var jsonDoc = JsonDocument.Parse(jsonString);
            return jsonDoc.RootElement.GetProperty("access_token").GetString()
                ?? throw new InvalidOperationException("Токен не найден в ответе");
        }
    }

    // Исключение для ошибок аутентификации
    public class AuthenticationFailureException : Exception
    {
        public AuthenticationFailureException(string message) : base(message) { }
    }
}
