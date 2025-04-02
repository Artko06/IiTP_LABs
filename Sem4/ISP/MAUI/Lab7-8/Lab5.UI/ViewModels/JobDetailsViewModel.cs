using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Lab5.Application.JobResponsibilitiesUseCases.Commands;
using Lab5.Application.JobResponsibilitiesUseCases.Queries;
using Lab5.UI.Pages;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Lab5.UI.ViewModels
{
    [QueryProperty("JobResponsibilities", "JobResponsibilities")]
    public partial class JobDetailsViewModel(IMediator mediator) : ObservableObject
    {
        [ObservableProperty] private JobResponsibilities jobResponsibilities;

        [ObservableProperty] string responsibilityName;
        [ObservableProperty] DateTime deadline;
        [ObservableProperty] int importance;
        [ObservableProperty] string status;

        [RelayCommand]
        async Task GetByIdJobResponsibilities()
        {
            JobResponsibilities = await mediator.Send(new GetByIdJobResponsibilitiesRequest(JobResponsibilities.Id));
            ResponsibilityName = JobResponsibilities.ResponsibilityName;
            Importance = JobResponsibilities.Importance;
            Deadline = JobResponsibilities.Deadline;
            Status = JobResponsibilities.Status;
        }

        [RelayCommand]
        async Task EditJobResponsibilities()
        {
            await Shell.Current.GoToAsync(nameof(AddOrEditJobResponsibilitiesWindow), new Dictionary<string, object>()
            {
                { "Request", new EditJobResponsibilitiesRequest() { JobResponsibilities = JobResponsibilities } }
            });
        }
    }
}
