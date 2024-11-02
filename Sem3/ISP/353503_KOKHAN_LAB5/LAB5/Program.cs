using SerializerLib;
using LAB5.Domain.Entities;
using Microsoft.Extensions.Configuration;
using System.Net;

internal class Program
{
    private static void Main(string[] args)
    {
        List<Company> companies = new();

        Company company1 = new Company("Google Inc");
        HRdepartment department1G = new("Central department", "San Francisco");
        HRdepartment department2G = new("Finance", "New York");
        
        Company company2 = new Company("Microsoft Corporation");
        HRdepartment department1M = new("Central department", "Washington");
        HRdepartment department2M = new("Research and Development", "New York");

        company1.AddDepartment(department1G);
        company1.AddDepartment(department2G);
        company2.AddDepartment(department1M);
        company2.AddDepartment(department2M);

        companies.Add(company1);
        companies.Add(company2);

        var builder = new ConfigurationBuilder()
                        .SetBasePath(Directory.GetCurrentDirectory())
                        .AddJsonFile("appsettings.json", optional: false, reloadOnChange: true);

        string? fileName = builder.Build()["FileSettings:FileName"];
        Console.WriteLine($"File Name: {fileName}");

        Serializer serializer = new Serializer();

        serializer.SerializeXML(companies, "Files/" + fileName + "_1" + ".xml");
        serializer.SerializeByLINQ(companies, "Files/" + fileName + "_2" + ".xml");
        serializer.SerializeJSON(companies, "Files/" + fileName + ".json");

        Console.WriteLine("Serialization completed.\n");

        var companiesJson = serializer.DeSerializeJSON("Files/" + fileName + ".json");
        var companiesXml = serializer.DeSerializeXML("Files/" + fileName + "_1" + ".xml");
        var companiesLinq = serializer.DeSerializeByLINQ("Files/" + fileName + "_2" + ".xml");

        bool areEqualJson = companies.SequenceEqual(companiesJson);
        bool areEqualXml = companies.SequenceEqual(companiesXml);
        bool areEqualLinq = companies.SequenceEqual(companiesLinq);

        Console.WriteLine("1. Deserialization from JSON\n");
        foreach (var item in companiesJson)
        {
            Console.WriteLine($"Company = {item.Name}");
            foreach (var department in item.Departments)
            {
                Console.WriteLine($"Department = {department.Name}, Place = {department.Place}");
            }
            Console.WriteLine();
        }

        Console.WriteLine("2. Deserialization from XML\n");
        foreach (var item in companiesXml)
        {
            Console.WriteLine($"Company = {item.Name}");
            foreach (var department in item.Departments)
            {
                Console.WriteLine($"Department = {department.Name}, Place = {department.Place}");
            }
            Console.WriteLine();
        }

        Console.WriteLine("3. Deserialization from XML by LINQ\n");
        foreach (var item in companiesLinq)
        {
            Console.WriteLine($"Company = {item.Name}");
            foreach (var department in item.Departments)
            {
                Console.WriteLine($"Department = {department.Name}, Place = {department.Place}");
            }
            Console.WriteLine();
        }

        Console.WriteLine($"JSON Deserialization matches: {areEqualJson}");
        Console.WriteLine($"XML Deserialization matches: {areEqualXml}");
        Console.WriteLine($"LINQ XML Deserialization matches: {areEqualLinq}");
    }
}