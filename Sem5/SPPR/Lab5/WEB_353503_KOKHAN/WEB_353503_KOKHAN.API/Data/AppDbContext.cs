using Microsoft.EntityFrameworkCore;
using WEB_353503_KOKHAN.Domain.Entities;

namespace WEB_353503_KOKHAN.API.Data
{
    public class AppDbContext : DbContext
    {
        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options)
        {
        }
        public DbSet<Category> Categories { get; set; }
        public DbSet<Car> Cars { get; set; }
    }
}
