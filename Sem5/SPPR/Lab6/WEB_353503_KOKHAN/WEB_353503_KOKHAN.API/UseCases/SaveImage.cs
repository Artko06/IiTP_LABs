using MediatR;

namespace WEB_353503_KOKHAN.API.UseCases
{
    public sealed record SaveImage(IFormFile File) : IRequest<string>;

    public class SaveImageHandler : IRequestHandler<SaveImage, string>
    {
        private readonly IWebHostEnvironment _env;
        private readonly IHttpContextAccessor _httpContextAccessor;
        private readonly ILogger<SaveImageHandler> _logger;

        public SaveImageHandler(
            IWebHostEnvironment env,
            IHttpContextAccessor httpContextAccessor,
            ILogger<SaveImageHandler> logger)
        {
            _env = env;
            _httpContextAccessor = httpContextAccessor;
            _logger = logger;
        }

        public async Task<string> Handle(SaveImage request, CancellationToken cancellationToken)
        {
            if (request.File == null || request.File.Length == 0)
            {
                _logger.LogWarning("Attempted to save null or empty file");
                return "Images/noimage.jpg";
            }

            try
            {
                // Генерация уникального имени файла
                var uniqueFileName = $"{Guid.NewGuid()}_{Path.GetFileName(request.File.FileName)}";
                var uploadsFolder = Path.Combine(_env.WebRootPath, "Images");

                // Создание папки если не существует
                if (!Directory.Exists(uploadsFolder))
                {
                    Directory.CreateDirectory(uploadsFolder);
                }

                var filePath = Path.Combine(uploadsFolder, uniqueFileName);

                // Сохранение файла
                using (var stream = new FileStream(filePath, FileMode.Create))
                {
                    await request.File.CopyToAsync(stream, cancellationToken);
                }

                // Формирование URL
                var httpRequest = _httpContextAccessor.HttpContext?.Request;
                var baseUrl = $"{httpRequest?.Scheme}://{httpRequest?.Host}";
                var imageUrl = $"{baseUrl}/Images/{uniqueFileName}";

                _logger.LogInformation($"Image saved successfully: {imageUrl}");
                return imageUrl;
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "Error saving image");
                throw;
            }
        }
    }
}