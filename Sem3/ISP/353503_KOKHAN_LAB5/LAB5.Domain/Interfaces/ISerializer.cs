using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using LAB5.Domain.Entities;

namespace LAB5.Domain.Interfaces
{
    public interface ISerializer
    {
        IEnumerable<Company> DeSerializeByLINQ(string fileName);
        IEnumerable<Company> DeSerializeXML(string fileName);
        IEnumerable<Company> DeSerializeJSON(string fileName);
        void SerializeByLINQ(IEnumerable<Company> list, string fileName);
        void SerializeXML(IEnumerable<Company> list, string fileName);
        void SerializeJSON(IEnumerable<Company> list, string fileName);
    }
}