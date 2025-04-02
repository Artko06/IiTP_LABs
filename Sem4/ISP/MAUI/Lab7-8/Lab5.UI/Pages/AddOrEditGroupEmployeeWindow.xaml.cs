using Lab5.UI.ViewModels;

namespace Lab5.UI.Pages;

public partial class AddOrEditGroupEmployeeWindow : ContentPage
{
	public AddOrEditGroupEmployeeWindow(AddGroupEmployeeViewModel addGroupEmployeeViewModel)
	{
		InitializeComponent();
        BindingContext = addGroupEmployeeViewModel;
    }
}