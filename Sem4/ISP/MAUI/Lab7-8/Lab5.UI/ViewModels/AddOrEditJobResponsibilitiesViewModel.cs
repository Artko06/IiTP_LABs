using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Lab5.Application.GroupEmployeeUseCases.Queries;
using Lab5.Application.JobResponsibilitiesUseCases.Commands;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.UI.ViewModels
{
    public partial class AddOrEditJobResponsibilitiesViewModel(IMediator mediator) : ObservableObject, IQueryAttributable
    {
        IAddOrEditJobResponsibilitiesRequest _request;

        [ObservableProperty] string responsibilityName = "";
        [ObservableProperty] DateTime deadline;
        [ObservableProperty] int importance = 1;
        [ObservableProperty] string status;

        public ObservableCollection<GroupEmployee> GroupsEmployee { get; set; } = new();
        [ObservableProperty] GroupEmployee? selectedGroupEmployee;
        [ObservableProperty] bool isGroupEmployeeIdChangeable = true;
        [ObservableProperty] string pickerTitle = "Select group employee";

        public async void ApplyQueryAttributes(IDictionary<string, object> query)
        {
            _request = query["Request"] as IAddOrEditJobResponsibilitiesRequest;
            if (_request == null)
                return;
            ResponsibilityName = _request.JobResponsibilities.ResponsibilityName;
            Deadline = _request.JobResponsibilities.Deadline;
            Importance = _request.JobResponsibilities.Importance;
            Status = _request.JobResponsibilities.Status;

            if (_request.JobResponsibilities.GroupEmployeeId != 0 && String.IsNullOrEmpty(_request.JobResponsibilities.ResponsibilityName))
            {
                SelectedGroupEmployee = await mediator.Send(new GetByIdGroupEmployeeRequest(_request.JobResponsibilities.GroupEmployeeId));
                IsGroupEmployeeIdChangeable = false;
                PickerTitle = "Group employee is selected";
            }
            else if (_request.JobResponsibilities.GroupEmployeeId != 0)
            {
                PickerTitle = $"Group employee with Id {_request.JobResponsibilities.GroupEmployeeId} was originally selected";
            }

            await GetGroupsEmployee();
        }

        [RelayCommand]
        public async Task GetGroupsEmployee()
        {
            var groupsEmployee = await mediator.Send(new GetAllGroupEmployeeRequest());
            await MainThread.InvokeOnMainThreadAsync(() =>
            {
                GroupsEmployee.Clear();
                foreach (var groupEmployee in groupsEmployee)
                    GroupsEmployee.Add(groupEmployee);
            });
        }

        [RelayCommand]
        public async Task AddOrEditJobResponsibilities()
        {
            if (String.IsNullOrEmpty(ResponsibilityName))
            {
                await Shell.Current.DisplayAlert("Error", "Name cannot be empty", "OK");
                return;
            }
            if (Importance < 1 || Importance > 10)
            {
                await Shell.Current.DisplayAlert("Error", "Importance must be between 1 and 10", "OK");
                return;
            }
            if (DateTime.Now > Deadline)
            {
                await Shell.Current.DisplayAlert("Error", "Deadline date cannot be earlier than now date", "OK");
                return;
            }

            _request.JobResponsibilities.ResponsibilityName = ResponsibilityName;
            _request.JobResponsibilities.Deadline = Deadline;
            _request.JobResponsibilities.Importance = Importance;
            _request.JobResponsibilities.GroupEmployeeId = SelectedGroupEmployee?.Id ?? _request.JobResponsibilities.GroupEmployeeId;
            _request.JobResponsibilities.Status = Status;
            await mediator.Send(_request);
            await GoBack();
        }

        [RelayCommand]
        async Task GoBack()
        {
            await Shell.Current.GoToAsync("..");
        }
    }

}
