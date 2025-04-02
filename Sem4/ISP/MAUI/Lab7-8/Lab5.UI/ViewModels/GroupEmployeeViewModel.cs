using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Lab5.Application.GroupEmployeeUseCases.Commands;
using Lab5.Application.GroupEmployeeUseCases.Queries;
using Lab5.Application.JobResponsibilitiesUseCases.Commands;
using Lab5.Application.JobResponsibilitiesUseCases.Queries;
using Lab5.UI.Pages;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.UI.ViewModels
{
    public partial class GroupEmployeeViewModel(IMediator mediator) : ObservableObject
    {
        private readonly IMediator _mediator = mediator;

        public ObservableCollection<GroupEmployee> GroupsEmployee { get; set; } = new();
        public ObservableCollection<JobResponsibilities> JobsResponsibilities { get; set; } = new();

        [ObservableProperty]
        GroupEmployee? selectedGroupEmployee;

        [RelayCommand]
        public async Task UpdateGroupList() => await GetGroupEmployee();

        [RelayCommand]
        public async Task UpdateMembersList() => await GetJobResponsibilities();

        [RelayCommand]
        public async Task ShowDetails(JobResponsibilities jobResponsibilities)
        {
            await GotoDetailsPage(jobResponsibilities);
            await GetJobResponsibilities();
        }

        [RelayCommand]
        public async Task AddGroupEmployee()
        {
            await GoToAddOrEditPage(nameof(AddOrEditGroupEmployeeWindow), new AddGroupEmployeeRequest() { GroupEmployee = new GroupEmployee() });
            await GetGroupEmployee();
        }

        [RelayCommand]
        public async Task AddJobResponsibilities()
        {
            if (SelectedGroupEmployee is null)
                return;
            await GoToAddOrEditPage(nameof(AddOrEditJobResponsibilitiesWindow),
                   new AddJobResponsibilitiesRequest() { JobResponsibilities = new JobResponsibilities() { GroupEmployeeId = SelectedGroupEmployee.Id } });
            await GetJobResponsibilities();
        }

        private async Task GetGroupEmployee()
        {
            var groupsEmployee = await _mediator.Send(new GetAllGroupEmployeeRequest());
            await MainThread.InvokeOnMainThreadAsync(() =>
            {
                GroupsEmployee.Clear();
                foreach (var groupEmployee in groupsEmployee)
                    GroupsEmployee.Add(groupEmployee);
            });
        }

        private async Task GetJobResponsibilities()
        {
            if (SelectedGroupEmployee == null) return;

            var jobs = await _mediator.Send(new GetByGroupJobResponsibilitiesRequest(SelectedGroupEmployee.Id));
            await MainThread.InvokeOnMainThreadAsync(() =>
            {
                JobsResponsibilities.Clear();
                foreach (var job in jobs)
                    JobsResponsibilities.Add(job);
            });
        }

        private async Task GotoDetailsPage(JobResponsibilities jobResponsibilities)
        {
            IDictionary<string, object> parameters =
                new Dictionary<string, object>()
                {
                    { "JobResponsibilities", jobResponsibilities}
                };
            await Shell.Current.GoToAsync(nameof(JobDetailsWindow), parameters);
        }

        private async Task GoToAddOrEditPage(string route, IRequest request)
        {
            await Shell.Current.GoToAsync(route, new Dictionary<string, object>()
            {
                { "Request", request }
            });
        }
    }
}
