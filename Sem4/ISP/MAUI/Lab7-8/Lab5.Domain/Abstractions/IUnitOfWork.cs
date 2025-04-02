using Lab5.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Domain.Abstractions
{
    public interface IUnitOfWork
    {
        IRepository<GroupEmployee> GroupEmployeeRepository { get; }
        IRepository<JobResponsibilities> JobResponsibilitiesRepository { get; }

        public Task SaveAllAsync();
        public Task DeleteDataBaseAsync();
        public Task CreateDataBaseAsync();
    }
}
