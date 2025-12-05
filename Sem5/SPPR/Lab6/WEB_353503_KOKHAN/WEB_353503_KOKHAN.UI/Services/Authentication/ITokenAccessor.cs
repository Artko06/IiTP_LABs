namespace WEB_353503_KOKHAN.UI.Services.Authentication
{
    public interface ITokenAccessor
    {
        /// <summary>
        /// Добавление заголовка Authorization : bearer
        /// </summary>
        /// <param name="httpClient">HttpClient, в который добавляется заголовок</param>
        /// <param name="isClient">если true – получить токен клиента; если false – получить токен пользователя</param>
        Task SetAuthorizationHeaderAsync(HttpClient httpClient, bool isClient);
    }
}
