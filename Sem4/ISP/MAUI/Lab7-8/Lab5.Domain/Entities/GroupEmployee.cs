using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Lab5.Domain.Entities
{
    public class GroupEmployee : Entity
    {
        public string NameGroup { get; set; }
        public List<JobResponsibilities> JobResponsibilities { get; set; } = new();

        // Фото сотрудника (в виде URL или пути к файлу)
        public string PhotoPath { get; set; }

        public decimal Salary { get; set; }  // Оклад
    }
}
