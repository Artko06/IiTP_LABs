namespace WEB_353503_KOKHAN.UI.Services.FileService
{
    public class LocalFileService : IFileService
    {
        private readonly IWebHostEnvironment _environment;
        private readonly string _imagesPath;

        public LocalFileService(IWebHostEnvironment environment)
        {
            _environment = environment;
            _imagesPath = Path.Combine(_environment.WebRootPath, "images");

            // Создаем папку если не существует
            if (!Directory.Exists(_imagesPath))
            {
                Directory.CreateDirectory(_imagesPath);
            }
        }

        public async Task<string> SaveFileAsync(IFormFile file)
        {
            if (file == null || file.Length == 0)
            {
                return "/images/default-profile-picture.png";
            }

            // Генерируем уникальное имя файла
            var fileName = $"{Guid.NewGuid()}{Path.GetExtension(file.FileName)}";
            var filePath = Path.Combine(_imagesPath, fileName);

            // Сохраняем файл
            using (var stream = new FileStream(filePath, FileMode.Create))
            {
                await file.CopyToAsync(stream);
            }

            return $"/images/{fileName}";
        }
    }
}