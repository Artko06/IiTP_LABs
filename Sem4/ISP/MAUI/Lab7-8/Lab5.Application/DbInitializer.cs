using Lab5.Domain.Entities;
using Microsoft.Extensions.DependencyInjection;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application
{
    public static class DbInitializer
    {
        public static async Task Initialize(IServiceProvider services)
        {
            var unitOfWork = services.GetRequiredService<IUnitOfWork>();
            await unitOfWork.DeleteDataBaseAsync();
            await unitOfWork.CreateDataBaseAsync();
            var random = new Random();

            for (int i = 1; i <= 3; i++)
            {
                await unitOfWork.GroupEmployeeRepository.AddAsync(new GroupEmployee
                {
                    Id = i,
                    NameGroup = $"Group {i}",
                    Salary = random.Next(20000, 100000),
                    PhotoPath = $"group{i}.png",
                    JobResponsibilities = []
                });
            };

            await unitOfWork.SaveAllAsync();

            for (int i = 1; i <= 3; ++i)
            {
                for (int j = 0; j < 10; ++j)
                {
                    await unitOfWork.JobResponsibilitiesRepository.AddAsync(new JobResponsibilities()
                    {
                        Id = (i - 1) * 10 + (j + 1),
                        ResponsibilityName = $"Work {(j + 1) + (i - 1) * 10}",
                        Importance = random.Next(1, 10),
                        Deadline = new DateTime(2025, random.Next(5, 12), random.Next(1, 30)),
                        GroupEmployeeId = i,
                        Status = "in Working"
                    });
                }
            }

            await unitOfWork.SaveAllAsync();
        }
    }
}
