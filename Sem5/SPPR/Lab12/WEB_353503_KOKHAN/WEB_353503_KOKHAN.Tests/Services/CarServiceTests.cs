 using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.API.Data;
using WEB_353503_KOKHAN.API.Services.CarService;
using WEB_353503_KOKHAN.Domain.Entities;

namespace WEB_353503_KOKHAN.Tests.Services
{
    public class CarServiceTests
    {
        private AppDbContext GetInMemoryDbContext()
        {
            var options = new DbContextOptionsBuilder<AppDbContext>()
                .UseInMemoryDatabase(databaseName: Guid.NewGuid().ToString())
                .Options;

            var context = new AppDbContext(options);

            // Seed test data с заполнением всех обязательных полей
            var categories = new List<Category>
            {
                new Category { Id = 1, Name = "Sedan", NormalizedName = "sedan" },
                new Category { Id = 2, Name = "SUV", NormalizedName = "suv" }
            };

            var cars = new List<Car>
            {
                new Car {
                    Id = 1,
                    Name = "Car 1",
                    Description = "Description 1",
                    Price = 10000,
                    CategoryId = 1,
                    Category = categories[0]
                },
                new Car {
                    Id = 2,
                    Name = "Car 2",
                    Description = "Description 2",
                    Price = 20000,
                    CategoryId = 1,
                    Category = categories[0]
                },
                new Car {
                    Id = 3,
                    Name = "Car 3",
                    Description = "Description 3",
                    Price = 30000,
                    CategoryId = 1,
                    Category = categories[0]
                },
                new Car {
                    Id = 4,
                    Name = "Car 4",
                    Description = "Description 4",
                    Price = 40000,
                    CategoryId = 2,
                    Category = categories[1]
                },
                new Car {
                    Id = 5,
                    Name = "Car 5",
                    Description = "Description 5",
                    Price = 50000,
                    CategoryId = 2,
                    Category = categories[1]
                }
            };

            context.Categories.AddRange(categories);
            context.Cars.AddRange(cars);
            context.SaveChanges();

            return context;
        }

        private AppDbContext GetEmptyInMemoryDbContext()
        {
            var options = new DbContextOptionsBuilder<AppDbContext>()
                .UseInMemoryDatabase(databaseName: Guid.NewGuid().ToString())
                .Options;

            return new AppDbContext(options);
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsFirstPageOfThreeItems()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync(null, 1, 3);

            // Assert
            Assert.True(result.Successfull);
            Assert.Equal(1, result.Data.CurrentPage);
            Assert.Equal(3, result.Data.Items.Count);
            Assert.Equal(2, result.Data.TotalPages); // 5 items total, 3 per page = 2 pages
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsCorrectPage()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync(null, 2, 3);

            // Assert
            Assert.True(result.Successfull);
            Assert.Equal(2, result.Data.CurrentPage);
            Assert.Equal(2, result.Data.Items.Count); // 2 items on second page
            Assert.Equal(2, result.Data.TotalPages);
        }

        [Fact]
        public async Task GetProductListAsync_FiltersByCategory()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync("sedan", 1, 10);

            // Assert
            Assert.True(result.Successfull);
            Assert.Equal(3, result.Data.Items.Count);
            Assert.All(result.Data.Items, car =>
                Assert.Equal("sedan", car.Category.NormalizedName));
        }

        [Fact]
        public async Task GetProductListAsync_LimitsPageSizeToMax()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync(null, 1, 25); // Try to set page size > max (20)

            // Assert
            Assert.True(result.Successfull);
            // Должен вернуть все 5 элементов, так как 25 > 20, но ограничивается максимумом 20
            // но у нас всего 5 элементов, поэтому вернет 5
            Assert.Equal(5, result.Data.Items.Count);
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsError_WhenPageExceedsTotalPages()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync(null, 10, 3); // Only 2 pages exist

            // Assert
            Assert.False(result.Successfull);
            Assert.Equal("No such page", result.ErrorMessage);
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsEmptyList_WhenNoItems()
        {
            // Arrange
            using var context = GetEmptyInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync("nonexistent", 1, 3);

            // Assert
            Assert.True(result.Successfull);
            Assert.Empty(result.Data.Items);
            Assert.Equal(1, result.Data.TotalPages);
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsCorrectTotalPages_ForExactDivision()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act - 5 элементов по 5 на странице = 1 страница
            var result = await service.GetProductListAsync(null, 1, 5);

            // Assert
            Assert.True(result.Successfull);
            Assert.Equal(1, result.Data.TotalPages);
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsCorrectTotalPages_ForNonExactDivision()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act - 5 элементов по 2 на странице = 3 страницы
            var result = await service.GetProductListAsync(null, 1, 2);

            // Assert
            Assert.True(result.Successfull);
            Assert.Equal(3, result.Data.TotalPages); // 5/2 = 2.5 → округляем вверх до 3
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsFirstPage_WhenPageNoIsZero()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync(null, 0, 3);

            // Assert
            Assert.True(result.Successfull);
            Assert.Equal(1, result.Data.CurrentPage); // Должен вернуть первую страницу
        }

        [Fact]
        public async Task GetProductListAsync_ReturnsFirstPage_WhenPageNoIsNegative()
        {
            // Arrange
            using var context = GetInMemoryDbContext();
            var service = new CarService(context);

            // Act
            var result = await service.GetProductListAsync(null, -1, 3);

            // Assert
            Assert.True(result.Successfull);
            Assert.Equal(1, result.Data.CurrentPage); // Должен вернуть первую страницу
        }
    }
}