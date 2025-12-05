using Serilog;

namespace WEB_353503_KOKHAN.Middleware
{
    public class ExceptionLoggingMiddleware
    {
        private readonly RequestDelegate _next;
        private readonly ILogger<ExceptionLoggingMiddleware> _logger;

        public ExceptionLoggingMiddleware(RequestDelegate next, ILogger<ExceptionLoggingMiddleware> logger)
        {
            _next = next;
            _logger = logger;
        }

        public async Task Invoke(HttpContext context)
        {
            try
            {
                await _next(context);

                if ((context.Response.StatusCode < 200 || context.Response.StatusCode >= 300) && context.Response.StatusCode != 500)
                {
                    _logger.LogInformation("---> request {Url} returns {StatusCode}", context.Request.Path, context.Response.StatusCode);
                }
            }
            catch (Exception)
            {
                _logger.LogInformation("---> request {Url} returns {StatusCode}", context.Request.Path, 500);

                throw;
            }
        }
    }
}