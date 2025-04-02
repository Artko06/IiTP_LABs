using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Persistense.Repository
{
    public class FakeJobResponsibilitiesRepository : IRepository<JobResponsibilities>
    {
        private readonly List<JobResponsibilities> _jobResponsibilities = new();

        public FakeJobResponsibilitiesRepository()
        {
            Random random = new Random();
            for (int i = 1; i <= 3; ++i)
                for (int j = 0; j < 10; ++j)
                {
                    _jobResponsibilities.Add(new JobResponsibilities()
                    {
                        Id = (i - 1) * 10 + (j + 1),
                        ResponsibilityName = $"Work {(j + 1)  + (i - 1) * 10}",
                        Importance = random.Next(0, 10),
                        Deadline = new DateTime(2025, random.Next(5, 12), random.Next(1, 30)),
                        GroupEmployeeId = i,
                        Status = "in Working"
                    });
                }
        }

        public async Task<IReadOnlyList<JobResponsibilities>> ListAsync(Expression<Func<JobResponsibilities, bool>>? filter,
            CancellationToken cancellationToken = default,
            params Expression<Func<JobResponsibilities, object>>[]? includesProperties)
        {
            IEnumerable<JobResponsibilities> query = _jobResponsibilities;
            if (filter != null)
            {
                query = query.Where(filter.Compile());
            }

            return await Task.FromResult(query.ToList());
        }

        public Task AddAsync(JobResponsibilities entity, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task DeleteAsync(JobResponsibilities entity, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task<JobResponsibilities> FirstOrDefaultAsync(Expression<Func<JobResponsibilities, bool>> filter, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task<JobResponsibilities> GetByIdAsync(int id, CancellationToken cancellationToken = default, params Expression<Func<JobResponsibilities, object>>[]? includesProperties)
        {
            throw new NotImplementedException();
        }

        public Task<IReadOnlyList<JobResponsibilities>> ListAllAsync(CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }

        public Task UpdateAsync(JobResponsibilities entity, CancellationToken cancellationToken = default)
        {
            throw new NotImplementedException();
        }
    }
}
