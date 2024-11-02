using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task2.Services
{
    static public class MyMath
    {
        static public double Foo(double z, double b)
        {
            double x = (z < 1) ? z / b : Math.Sqrt(z * z * z * b * b * b);

            Console.WriteLine((z < 1) ? $"On the first branch, because z < 1, x = {x}" :
                $"On the second branch, because z >= 1, x = {x}");

            return -Math.PI + Math.Cos(x * x * x) * Math.Cos(x * x * x) + Math.Sin(x * x) *
                Math.Sin(x * x) * Math.Sin(x * x);
        }
    }
}
