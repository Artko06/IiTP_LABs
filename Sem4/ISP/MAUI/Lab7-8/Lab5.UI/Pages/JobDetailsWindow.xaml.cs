namespace Lab5.UI.Pages;

public partial class JobDetailsWindow : ContentPage
{
	public JobDetailsWindow(ViewModels.JobDetailsViewModel jobDetailsViewModel)
	{
		InitializeComponent();
		BindingContext = jobDetailsViewModel;
	}
}