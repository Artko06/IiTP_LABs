using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LAB6.Entities
{
    public class Employee
    {
        private string _name;
        private int _age;
        private bool _working;

        public string Name{get { return _name; } set { _name = value; } }
        public int Age { get { return _age; } set { _age = value; } }
        public bool Working { get { return _working; } set { _working = value; } }

        public Employee(string? name, int age, bool working)
        {
            _name = name ?? string.Empty;
            _age = age;
            _working = working;
        }
    }
}