namespace WEB_353503_KOKHAN.UI.Services.CategoryService
{
    public class ApiCategoryService : ICategoryService
    {
        private readonly HttpClient _httpClient;

        public ApiCategoryService(HttpClient httpClient)
        {
            _httpClient = httpClient;
        }

        public async Task<ResponseData<List<Category>>> GetCategoryListAsync()
        {
            return await _httpClient.GetFromJsonAsync<ResponseData<List<Category>>>("categories");
        }
    }
}
