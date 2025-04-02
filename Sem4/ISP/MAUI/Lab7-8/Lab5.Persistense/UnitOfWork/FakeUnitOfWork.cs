using Lab5.Persistense.Repository;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Persistense.UnitOfWork
{
    public class FakeUnitOfWork : IUnitOfWork
    {
        private readonly FakeGroupEmployee _fakeGroupEmployee = new();
        private readonly FakeJobResponsibilitiesRepository _fakeJobResponsibilitiesRepository = new();

        public IRepository<GroupEmployee> GroupEmployeeRepository => _fakeGroupEmployee;

        public IRepository<JobResponsibilities> JobResponsibilitiesRepository => _fakeJobResponsibilitiesRepository;

        public Task SaveAllAsync()
        {
            throw new NotImplementedException();
        }

        public Task DeleteDataBaseAsync()
        {
            throw new NotImplementedException();
        }

        public Task CreateDataBaseAsync()
        {
            throw new NotImplementedException();
        }
    }
}
