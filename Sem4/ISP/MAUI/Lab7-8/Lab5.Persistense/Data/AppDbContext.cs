using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Persistense.Data
{
    public class AppDbContext : DbContext
    {
        public DbSet<GroupEmployee> groupsEmployee { get; }
        public DbSet<JobResponsibilities> jobs { get; }

        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options)
        {
            Database.EnsureCreated();
        }
    }
}
