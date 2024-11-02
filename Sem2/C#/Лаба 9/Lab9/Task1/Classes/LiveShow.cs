using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Task1.Interfaces;

namespace Task1.Classes
{
    internal class LiveShow : IShowType
    {
        public string Show() =>
            "Показывает прямую трансляцию";
    }
}
