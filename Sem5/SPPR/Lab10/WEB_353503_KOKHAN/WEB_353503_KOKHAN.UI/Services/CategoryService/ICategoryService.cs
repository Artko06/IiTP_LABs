namespace WEB_353503_KOKHAN.UI.Services.CategoryService
{
    public interface ICategoryService
    {
        public Task<ResponseData<List<Category>>> GetCategoryListAsync();
    }
}
