using Lab5.Persistense.Data;
using Lab5.Persistense.Repository;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Persistense.UnitOfWork
{
    public class EfUnitOfWork(AppDbContext context) : IUnitOfWork
    {
        private readonly Lazy<IRepository<JobResponsibilities>> _workRepository = new(() => new EfRepository<JobResponsibilities>(context));
        private readonly Lazy<IRepository<GroupEmployee>> _brigadeRepository = new(() => new EfRepository<GroupEmployee>(context));

        public IRepository<JobResponsibilities> JobResponsibilitiesRepository => _workRepository.Value;
        public IRepository<GroupEmployee> GroupEmployeeRepository => _brigadeRepository.Value;

        public async Task SaveAllAsync() => await context.SaveChangesAsync();

        public async Task DeleteDataBaseAsync() => await context.Database.EnsureDeletedAsync();

        public async Task CreateDataBaseAsync() => await context.Database.EnsureCreatedAsync();
    }
}
