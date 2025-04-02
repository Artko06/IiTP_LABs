using Lab5.UI.ViewModels;

namespace Lab5.UI.Pages;

public partial class AddOrEditJobResponsibilitiesWindow : ContentPage
{
	public AddOrEditJobResponsibilitiesWindow(AddOrEditJobResponsibilitiesViewModel addOrEditJobResponsibilitiesViewModel)
	{
		InitializeComponent();
		BindingContext = addOrEditJobResponsibilitiesViewModel;
    }
}