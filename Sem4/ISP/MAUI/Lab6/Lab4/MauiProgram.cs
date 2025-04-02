using Lab4.Pages;
using Lab4.Services;
using Lab4.Services.Http;
using Microsoft.Extensions.Logging;

namespace Lab4
{
    public static class MauiProgram
    {
        public static MauiApp CreateMauiApp()
        {
            var builder = MauiApp.CreateBuilder();
            builder
                .UseMauiApp<App>()
                .ConfigureFonts(fonts =>
                {
                    fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                    fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                });

            builder.Services.AddTransient<IDbService, SQLiteService>();
            builder.Services.AddTransient<SQLitePage>();

            builder.Services.AddTransient<IRateService, RateService>();
            builder.Services.AddTransient<ConverterPage>();
            builder.Services.AddHttpClient("NB RB", opt => opt.BaseAddress = new Uri("https://www.nbrb.by/api/exrates/rates"));

#if DEBUG
            builder.Logging.AddDebug();
#endif

            return builder.Build();
        }
    }
}
