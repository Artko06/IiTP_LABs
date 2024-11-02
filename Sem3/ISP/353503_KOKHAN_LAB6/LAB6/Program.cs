using System.Collections;
using System.Reflection;
using LAB6.Entities;

internal class Program
{
    private static void Main(string[] args)
    {
        const string fileName = "Files/file.json";

        List<Employee> listEmployees = new() {new Employee("John", 30, true),
                                              new Employee("Lina", 44, true),
                                              new Employee("Jane", 28, false),
                                              new Employee("Adam", 55, true),
                                              new Employee("Emily", 50, false)
                                              };

        Assembly asm = Assembly.LoadFrom("../DllLib/bin/Debug/net8.0/DllLib.dll"); 
        Type? type = asm.GetType("DllLib.FileService");

        if (type is not null)
        {
            object? instance = Activator.CreateInstance(type);

            MethodInfo? saveData = type?.GetMethod("SaveData");
            MethodInfo? readFile = type?.GetMethod("ReadFile");

            saveData?.Invoke(instance, new object[] { listEmployees, fileName });
            var readEmployees = readFile?.Invoke(instance, new object[] { fileName });

            Console.WriteLine("Employees from JSON file:");
            foreach (Employee employee in (IEnumerable<Employee>?)readEmployees ?? Enumerable.Empty<Employee>())
            {
                Console.WriteLine($"Name: {employee.Name}, Age: {employee.Age}, Working: {employee.Working}");
            }
        }
    }
}