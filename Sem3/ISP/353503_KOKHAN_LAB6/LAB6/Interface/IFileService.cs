using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LAB6.Interface
{
    public interface IFileService<T> where T : class
    {
        IEnumerable<T> ReadFile(string fileName);
        void SaveData(IEnumerable<T> data, string fileName);
    }
}