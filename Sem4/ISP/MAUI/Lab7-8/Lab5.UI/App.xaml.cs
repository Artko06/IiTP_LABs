namespace Lab5.UI
{
    public partial class App : Microsoft.Maui.Controls.Application
    {
        public App()
        {
            InitializeComponent();

            MainPage = new AppShell();

            Routing.RegisterRoute(nameof(Pages.JobDetailsWindow), typeof(Pages.JobDetailsWindow));
            Routing.RegisterRoute(nameof(Pages.AddOrEditGroupEmployeeWindow), typeof(Pages.AddOrEditGroupEmployeeWindow));
            Routing.RegisterRoute(nameof(Pages.AddOrEditJobResponsibilitiesWindow), typeof(Pages.AddOrEditJobResponsibilitiesWindow));
        }
    }
}
