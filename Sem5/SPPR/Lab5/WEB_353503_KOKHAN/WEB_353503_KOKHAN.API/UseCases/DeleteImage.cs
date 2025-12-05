using MediatR;

namespace WEB_353503_KOKHAN.API.UseCases
{
    public sealed record DeleteImage(string ImageUrl) : IRequest<bool>;

    public class DeleteImageHandler : IRequestHandler<DeleteImage, bool>
    {
        private readonly IWebHostEnvironment _env;
        private readonly ILogger<DeleteImageHandler> _logger;

        public DeleteImageHandler(
            IWebHostEnvironment env,
            ILogger<DeleteImageHandler> logger)
        {
            _env = env;
            _logger = logger;
        }

        public Task<bool> Handle(DeleteImage request, CancellationToken cancellationToken)
        {
            if (string.IsNullOrEmpty(request.ImageUrl) ||
                request.ImageUrl.Contains("noimage.jpg") ||
                !request.ImageUrl.Contains("/Images/"))
            {
                return Task.FromResult(true); 
            }

            try
            {
                // Извлечение имени файла из URL
                var fileName = Path.GetFileName(request.ImageUrl);
                if (string.IsNullOrEmpty(fileName))
                {
                    return Task.FromResult(false);
                }

                var filePath = Path.Combine(_env.WebRootPath, "Images", fileName);

                if (File.Exists(filePath))
                {
                    File.Delete(filePath);
                    _logger.LogInformation($"Image deleted successfully: {filePath}");
                    return Task.FromResult(true);
                }

                _logger.LogWarning($"Image file not found: {filePath}");
                return Task.FromResult(false);
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "Error deleting image");
                return Task.FromResult(false);
            }
        }
    }
}