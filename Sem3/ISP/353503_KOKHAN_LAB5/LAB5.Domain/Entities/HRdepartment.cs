using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LAB5.Domain.Entities
{
    public class HRdepartment : IEquatable<HRdepartment>
    {
        private string name;
        private string place;

        public string Name { get { return name; } set { name = value; } }

        public string Place { get { return place; } set { place = value; } }

        public HRdepartment()
        {
            name = string.Empty;
            place = string.Empty;
        }

        public HRdepartment(string? name, string? place)
        {
            this.name = name ?? string.Empty;
            this.place =  place ?? string.Empty;
        }

        public bool Equals(HRdepartment? other)
        {
            if (other == null) return false;
            return Name == other.Name && Place == other.Place;
        }

        public override bool Equals(object? obj) => Equals(obj as HRdepartment);
        public override int GetHashCode() => HashCode.Combine(Name, Place);
    }
}