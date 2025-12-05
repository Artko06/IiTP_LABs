namespace WEB_353503_KOKHAN.UI.Services.FileService
{
    public interface IFileService
    {
        /// <summary>
        /// Сохраняет данные в файле
        /// </summary>
        /// <param name="file">данные для сохранения</param>
        /// <returns>адрес сохраненного файла</returns>
        Task<string> SaveFileAsync(IFormFile file);
    }
}
