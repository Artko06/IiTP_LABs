using Lab5.UI.Pages;
using Lab5.UI.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.UI
{
    public static class DependencyInjection
    {
        public static IServiceCollection RegisterPages(this IServiceCollection services)
        {
            services
                .AddSingleton<GroupEmployeeWindow>()
                .AddTransient<JobDetailsWindow>()
                .AddTransient<AddOrEditGroupEmployeeWindow>()
                .AddTransient<AddOrEditJobResponsibilitiesWindow>();
            return services;
        }

        public static IServiceCollection RegisterViewModels(this IServiceCollection services)
        {
            services
                .AddSingleton<GroupEmployeeViewModel>()
                .AddTransient<JobDetailsViewModel>()
                .AddTransient<AddGroupEmployeeViewModel>()
                .AddTransient<AddOrEditJobResponsibilitiesViewModel>();
            return services;
        }
    }
}
