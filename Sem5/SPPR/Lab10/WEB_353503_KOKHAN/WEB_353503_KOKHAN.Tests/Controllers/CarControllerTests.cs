using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.ViewFeatures;
using Microsoft.Extensions.Configuration;
using NSubstitute;
using WEB_353503_KOKHAN.Domain.Entities;
using WEB_353503_KOKHAN.Domain.Models;
using WEB_353503_KOKHAN.UI.Controllers;
using WEB_353503_KOKHAN.UI.Services.CarService;
using WEB_353503_KOKHAN.UI.Services.CategoryService;

namespace WEB_353503_KOKHAN.Tests.Controllers
{
    public class CarControllerTests
    {
        private readonly CarController _controller;
        private readonly ICarService _carService;
        private readonly ICategoryService _categoryService;
        private readonly IConfiguration _configuration;

        public CarControllerTests()
        {
            _carService = Substitute.For<ICarService>();
            _categoryService = Substitute.For<ICategoryService>();
            _configuration = Substitute.For<IConfiguration>();

            _controller = new CarController(_carService, _categoryService, _configuration);

            // Полная настройка HttpContext и сервисов MVC
            var httpContext = Substitute.For<HttpContext>();
            var request = Substitute.For<HttpRequest>();
            var response = Substitute.For<HttpResponse>();
            var session = Substitute.For<ISession>();
            var serviceProvider = Substitute.For<IServiceProvider>();

            // Настраиваем необходимые сервисы MVC
            var tempDataProvider = Substitute.For<ITempDataProvider>();
            var tempDataDictionaryFactory = Substitute.For<ITempDataDictionaryFactory>();
            tempDataDictionaryFactory.GetTempData(Arg.Any<HttpContext>()).Returns(new TempDataDictionary(httpContext, tempDataProvider));

            serviceProvider.GetService(typeof(ITempDataDictionaryFactory)).Returns(tempDataDictionaryFactory);

            httpContext.Request.Returns(request);
            httpContext.Response.Returns(response);
            httpContext.Session.Returns(session);
            httpContext.RequestServices.Returns(serviceProvider);

            _controller.ControllerContext = new ControllerContext
            {
                HttpContext = httpContext
            };

            // Настраиваем ViewData и TempData
            _controller.ViewData = new ViewDataDictionary(new Microsoft.AspNetCore.Mvc.ModelBinding.EmptyModelMetadataProvider(), new Microsoft.AspNetCore.Mvc.ModelBinding.ModelStateDictionary());
            _controller.TempData = new TempDataDictionary(httpContext, tempDataProvider);
        }

        [Fact]
        public async Task Index_ReturnsNotFound_WhenCategoriesNotLoaded()
        {
            // Arrange
            var categoryResponse = new ResponseData<List<Category>>
            {
                Successfull = false,
                ErrorMessage = "Error loading categories"
            };
            _categoryService.GetCategoryListAsync().Returns(categoryResponse);

            // Act
            var result = await _controller.Index(null);

            // Assert
            var notFoundResult = Assert.IsType<NotFoundObjectResult>(result);
            Assert.Equal("Categories could not be loaded.", notFoundResult.Value);
        }

        [Fact]
        public async Task Index_ReturnsNotFound_WhenProductsNotLoaded()
        {
            // Arrange
            var categories = new List<Category>
            {
                new Category { Id = 1, Name = "Sedan", NormalizedName = "sedan" }
            };
            var categoryResponse = new ResponseData<List<Category>>
            {
                Successfull = true,
                Data = categories
            };
            _categoryService.GetCategoryListAsync().Returns(categoryResponse);

            var productResponse = new ResponseData<ListModel<Car>>
            {
                Successfull = false,
                ErrorMessage = "Error loading products"
            };
            _carService.GetProductListAsync(Arg.Any<string>(), Arg.Any<int>()).Returns(productResponse);

            // Act
            var result = await _controller.Index(null);

            // Assert
            var notFoundResult = Assert.IsType<NotFoundObjectResult>(result);
            Assert.Equal("Error loading products", notFoundResult.Value);
        }

        [Fact]
        public async Task Index_ReturnsViewWithCorrectData_WhenSuccess()
        {
            // Arrange
            var categories = new List<Category>
            {
                new Category { Id = 1, Name = "Sedan", NormalizedName = "sedan" },
                new Category { Id = 2, Name = "SUV", NormalizedName = "suv" }
            };
            var categoryResponse = new ResponseData<List<Category>>
            {
                Successfull = true,
                Data = categories
            };
            _categoryService.GetCategoryListAsync().Returns(categoryResponse);

            var cars = new ListModel<Car>
            {
                Items = new List<Car>
                {
                    new Car { Id = 1, Name = "Car 1", CategoryId = 1 },
                    new Car { Id = 2, Name = "Car 2", CategoryId = 1 }
                },
                CurrentPage = 1,
                TotalPages = 1
            };
            var productResponse = new ResponseData<ListModel<Car>>
            {
                Successfull = true,
                Data = cars
            };
            _carService.GetProductListAsync(Arg.Any<string>(), Arg.Any<int>()).Returns(productResponse);

            // Act
            var result = await _controller.Index(null);

            // Assert
            var viewResult = Assert.IsType<ViewResult>(result);
            Assert.Equal(cars, viewResult.Model);

            // Проверяем ViewBag/ViewData
            Assert.Equal(categories, viewResult.ViewData["Categories"] ?? _controller.ViewBag.Categories);
            Assert.Equal("Все Автомобили", viewResult.ViewData["CurrentCategory"] ?? _controller.ViewBag.CurrentCategory);
        }

        [Fact]
        public async Task Index_SetsCorrectCurrentCategory_WhenCategoryProvided()
        {
            // Arrange
            var categories = new List<Category>
            {
                new Category { Id = 1, Name = "Sedan", NormalizedName = "sedan" }
            };
            var categoryResponse = new ResponseData<List<Category>>
            {
                Successfull = true,
                Data = categories
            };
            _categoryService.GetCategoryListAsync().Returns(categoryResponse);

            var cars = new ListModel<Car>
            {
                Items = new List<Car>(),
                CurrentPage = 1,
                TotalPages = 1
            };
            var productResponse = new ResponseData<ListModel<Car>>
            {
                Successfull = true,
                Data = cars
            };
            _carService.GetProductListAsync(Arg.Any<string>(), Arg.Any<int>()).Returns(productResponse);

            // Act
            var result = await _controller.Index("sedan");

            // Assert
            var viewResult = Assert.IsType<ViewResult>(result);
            Assert.Equal("Sedan", viewResult.ViewData["CurrentCategory"] ?? _controller.ViewBag.CurrentCategory);
        }

        [Fact]
        public async Task Index_ReturnsPartialView_ForAjaxRequest()
        {
            // Arrange
            var categories = new List<Category>
            {
                new Category { Id = 1, Name = "Sedan", NormalizedName = "sedan" }
            };
            var categoryResponse = new ResponseData<List<Category>>
            {
                Successfull = true,
                Data = categories
            };
            _categoryService.GetCategoryListAsync().Returns(categoryResponse);

            var cars = new ListModel<Car>
            {
                Items = new List<Car>(),
                CurrentPage = 1,
                TotalPages = 1
            };
            var productResponse = new ResponseData<ListModel<Car>>
            {
                Successfull = true,
                Data = cars
            };
            _carService.GetProductListAsync(Arg.Any<string>(), Arg.Any<int>()).Returns(productResponse);

            // Настраиваем AJAX запрос
            _controller.ControllerContext.HttpContext.Request.Headers["X-Requested-With"] = "XMLHttpRequest";

            // Act
            var result = await _controller.Index(null);

            // Assert
            var partialViewResult = Assert.IsType<PartialViewResult>(result);
            Assert.Equal("~/Views/Shared/Components/Car/_CarListPartial.cshtml", partialViewResult.ViewName);
            Assert.Equal(cars, partialViewResult.Model);
        }
    }
}