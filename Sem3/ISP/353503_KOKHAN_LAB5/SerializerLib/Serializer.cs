using System.Text.Json;
using System.Xml.Serialization;

using LAB5.Domain.Interfaces;
using LAB5.Domain.Entities;
using System.Xml.Linq;

namespace SerializerLib
{
    public class Serializer : ISerializer
    {
        public IEnumerable<Company> DeSerializeByLINQ(string fileName)
        {
            XDocument xDocument = XDocument.Load(fileName);

            var companies = xDocument?.Element("Companies")
                          ?.Elements("Company")
                            .Select(companyEl => new Company(companyEl.Element("Name")?.Value, 
                                    companyEl?.Element("Departments")
                                    ?.Elements("Department")
                                    .Select(departmentEl => new HRdepartment(departmentEl.Element("Name")?.Value,
                                    departmentEl?.Element("Place")?.Value)).ToList() ?? new List<HRdepartment>()
                                    ));

            return companies ?? Enumerable.Empty<Company>();
        }

        public IEnumerable<Company> DeSerializeXML(string fileName)
        {
            XmlSerializer xmlSerializer = new XmlSerializer(typeof(List<Company>));

            using (FileStream fs = new FileStream(fileName, FileMode.Open))
            {   
                return (List<Company>?)xmlSerializer.Deserialize(fs) ?? Enumerable.Empty<Company>();
            }
        }

        public IEnumerable<Company> DeSerializeJSON(string fileName)
        {
            return JsonSerializer.Deserialize<List<Company>>(File.ReadAllText(fileName)) ?? Enumerable.Empty<Company>();
        }

        public void SerializeByLINQ(IEnumerable<Company> list, string fileName)
        {
            XDocument xDocument = new XDocument();

            XElement root = new("Companies");

            foreach (var company in list)
            {
                XElement companyElement = new XElement("Company",
                                    new XElement("Name", company.Name),
                                    new XElement("Departments",
                                    from department in company.Departments
                                    select new XElement("Department",
                                    new XElement("Name", department.Name),
                                    new XElement("Place", department.Place)
                                    )));

                root.Add(companyElement);
            }

            xDocument.Add(root);
            xDocument.Save(fileName);
        }

        public void SerializeXML(IEnumerable<Company> list, string fileName)
        {     
            XmlSerializer xmlSerializer = new XmlSerializer(typeof(List<Company>));

            using (FileStream fs = new FileStream(fileName, FileMode.Create))
            {
                xmlSerializer.Serialize(fs, list);
            }
        }

        public void SerializeJSON(IEnumerable<Company> list, string fileName)
        {
            File.WriteAllText(fileName, JsonSerializer.Serialize(list));
        }
    }
}