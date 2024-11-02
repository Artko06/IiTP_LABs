using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Task1.Interfaces;

namespace Task1.Classes
{
    internal abstract class TV
    {
        public string? nameTV { get; set; }
        public string? screenType {  get; set; }
        public IShowType? showType { get; set; } = null;

        public abstract string GetInfo();
        public abstract string Show();
    }
}
