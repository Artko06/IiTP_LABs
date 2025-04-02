using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Lab5.Application.GroupEmployeeUseCases.Commands;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Lab5.UI.ViewModels
{
    public partial class AddGroupEmployeeViewModel(IMediator mediator) : ObservableObject, IQueryAttributable
    {
        IAddOrEditGroupEmployeeRequest _request;
        private static int _imagesCounter = 0;
        private const string DEFAULT_IMAGE = "default_image.jpg";

        [ObservableProperty] string nameGroup = "";
        [ObservableProperty] string photoPath = DEFAULT_IMAGE;
        [ObservableProperty] decimal salary = -1;


        public void ApplyQueryAttributes(IDictionary<string, object> query)
        {
            _request = query["Request"] as IAddOrEditGroupEmployeeRequest;
            if (_request == null)
                return;
            NameGroup = _request.GroupEmployee.NameGroup;
            Salary = _request.GroupEmployee.Salary;
            PhotoPath = _request.GroupEmployee.PhotoPath ?? PhotoPath;
            _request.GroupEmployee.PhotoPath = _request.GroupEmployee.PhotoPath ?? DEFAULT_IMAGE;
        }

        [RelayCommand]
        public async Task AddOrEditGroupEmployee()
        {
            if (String.IsNullOrEmpty(NameGroup))
            {
                await Shell.Current.DisplayAlert("Error", "Name cannot be empty.", "OK");
                return;
            }
            if (String.IsNullOrEmpty(PhotoPath))
            {
                await Shell.Current.DisplayAlert("Error", "PhotoPath cannot be empty.", "OK");
                return;
            }
            if (Salary < 0)
            {
                await Shell.Current.DisplayAlert("Error", "Salary cannot be negative.", "OK");
                return;
            }

            _request.GroupEmployee.NameGroup = NameGroup;
            _request.GroupEmployee.Salary = Salary;
            _request.GroupEmployee.PhotoPath = PhotoPath;
            await mediator.Send(_request);
            await GoBack();
        }

        [RelayCommand]
        async Task GoBack()
        {
            await Shell.Current.GoToAsync("..");
        }

        [RelayCommand]
        async Task SelectImage()
        {
            var result = await FilePicker.PickAsync(new PickOptions
            {
                FileTypes = FilePickerFileType.Images,
                PickerTitle = "Choose image"
            });

            if (result != null)
            {
                string targetFolder = Path.Combine(FileSystem.AppDataDirectory, "Images");
                Directory.CreateDirectory(targetFolder);

                string targetFile = Path.Combine(targetFolder, $"image{++_imagesCounter}.jpg");

                using (var sourceStream = await result.OpenReadAsync())
                using (var destinationStream = File.Create(targetFile))
                {
                    await sourceStream.CopyToAsync(destinationStream);
                }
                PhotoPath = targetFile;
            }
        }
    }
}
