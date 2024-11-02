using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using _353503_KOKHAN_LAB4.Entities;

namespace _353503_KOKHAN_LAB4.Comparers
{
    public class MyCustomComparer : IComparer<StoreGoods>
    {
        public int Compare(StoreGoods? left, StoreGoods? right)
        {
            return string.Compare(left?.Name, right?.Name);
        }
    }
}