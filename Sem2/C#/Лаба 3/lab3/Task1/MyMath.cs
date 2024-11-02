using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1
{
    static public class MyMath
    {
        static public long Operation(long numberValue)
        {
            return (numberValue % 2 == 0) ? numberValue / 2 : 0;
        }
    }
}
