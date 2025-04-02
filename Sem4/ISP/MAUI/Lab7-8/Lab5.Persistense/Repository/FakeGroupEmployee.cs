using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Persistense.Repository
{
    public class FakeGroupEmployee : IRepository<GroupEmployee>
    {
        private readonly List<GroupEmployee> _groupEmployee;

        public FakeGroupEmployee()
        {
            _groupEmployee = new List<GroupEmployee>
            {
                new()
                {
                    Id = 1,
                    NameGroup = "Group 1",
                    Salary = 14500,
                    PhotoPath = "group1.png",
                    JobResponsibilities = []
                },
                new()
                {
                    Id = 2,
                    NameGroup = "Group 2",
                    Salary = 21200,
                    PhotoPath = "group2.png",
                    JobResponsibilities = []
                },
                new()
                {
                    Id = 3,
                    NameGroup = "Group 3",
                    Salary = 121450,
                    PhotoPath = "group3.png",
                    JobResponsibilities = []
                }
            };
        }
        public async Task<IReadOnlyList<GroupEmployee>> ListAllAsync(CancellationToken cancellationToken = default)
        {
            return await Task.Run(() => (IReadOnlyList<GroupEmployee>)_groupEmployee.AsReadOnly(), cancellationToken);
        }

        public Task<GroupEmployee> GetByIdAsync(int id, CancellationToken cancellationToken = default,
            params Expression<Func<GroupEmployee, object>>[]? includesProperties)
        {
            throw new NotImplementedException();
        }

        public Task<IReadOnlyList<GroupEmployee>> ListAsync(Expression<Func<GroupEmployee, bool>> filter,
            CancellationToken cancellationToken = default, params Expression<Func<GroupEmployee, object>>[]? includesProperties)
        {
            throw new NotImplementedException();
        }

        public Task AddAsync(GroupEmployee entity, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task UpdateAsync(GroupEmployee entity, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task DeleteAsync(GroupEmployee entity, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task<GroupEmployee> FirstOrDefaultAsync(Expression<Func<GroupEmployee, bool>> filter,
            CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }
    }
}
