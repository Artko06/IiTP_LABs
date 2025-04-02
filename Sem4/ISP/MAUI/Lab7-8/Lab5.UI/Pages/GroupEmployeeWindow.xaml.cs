namespace Lab5.UI.Pages;

public partial class GroupEmployeeWindow : ContentPage
{
	public GroupEmployeeWindow(ViewModels.GroupEmployeeViewModel viewGroupEmployeeViewModel)
	{
		InitializeComponent();
        BindingContext = viewGroupEmployeeViewModel;
    }
}