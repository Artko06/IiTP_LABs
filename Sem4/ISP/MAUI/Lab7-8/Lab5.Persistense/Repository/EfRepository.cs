using Lab5.Persistense.Data;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Persistense.Repository
{
    public class EfRepository<T>(AppDbContext context) : IRepository<T>
    where T : Entity
    {
        protected readonly AppDbContext Context = context;
        protected readonly DbSet<T> Entities = context.Set<T>();

        public async Task<T> GetByIdAsync(int id,
            CancellationToken cancellationToken = default,
            params Expression<Func<T, object>>[]? includesProperties
        )
        {
            var query = Entities.AsQueryable();
            if (includesProperties is null || includesProperties.Length == 0)
                return (await query.Where((e) => e.Id == id).FirstOrDefaultAsync(cancellationToken))!;
            query = includesProperties.Aggregate(query, (current, included) => current.Include(included));

            return (await query.Where((e) => e.Id == id).FirstOrDefaultAsync(cancellationToken))!;
        }

        public async Task<IReadOnlyList<T>> ListAllAsync(CancellationToken cancellationToken = default)
        {
            return await Entities.ToListAsync(cancellationToken);
        }

        public async Task<IReadOnlyList<T>> ListAsync(Expression<Func<T, bool>>? filter,
            CancellationToken cancellationToken = default,
            params Expression<Func<T, object>>[]? includesProperties)
        {
            IQueryable<T>? query = Entities.AsQueryable();
            if (includesProperties is null)
            {
                return await query.ToListAsync(cancellationToken: cancellationToken);
            }

            if (includesProperties.Length != 0)
            {
                query = includesProperties.Aggregate(query, (current, included) => current.Include(included));
            }

            if (filter != null)
            {
                query = query.Where(filter);
            }

            return await query.ToListAsync(cancellationToken: cancellationToken);
        }

        public Task AddAsync(T entity, CancellationToken cancellationToken = default)
        {
            Entities.Add(entity);
            return Task.CompletedTask;
        }

        public Task DeleteAsync(T entity, CancellationToken cancellationToken = default)
        {
            Entities.Remove(entity);
            return Task.CompletedTask;
        }

        public async Task<T> FirstOrDefaultAsync(Expression<Func<T, bool>> filter,
            CancellationToken cancellationToken = default)
        {
            return (await Entities.FirstOrDefaultAsync(filter, cancellationToken))!;
        }

        public Task UpdateAsync(T entity, CancellationToken cancellationToken = default)
        {
            Context.Entry(entity).State = EntityState.Modified;
            return Task.CompletedTask;
        }
    }
}
