using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LAB5.Domain.Entities
{
    public class Company : IEquatable<Company>
    {   
        private string name;

        public string Name { get { return name; } set { name = value; } }

        public List<HRdepartment> Departments { get; set; }

        public Company()
        {
            Departments = new();
            name = string.Empty;
        }

        public Company(string? name)
        {
            this.name = name ?? string.Empty;
            Departments = new();
        }
        public Company(string? name, List<HRdepartment> list)
        {
            this.name = name ?? string.Empty;
            Departments = list;
        }

        public void AddDepartment(HRdepartment department)
        {
            Departments.Add(department);
        }

        public bool Equals(Company? other)
        {
            if (other == null) return false;
            return Name == other.Name && Departments.SequenceEqual(other.Departments);
        }

        public override bool Equals(object? obj) => Equals(obj as Company);
        public override int GetHashCode() => HashCode.Combine(Name, Departments);
    }
}