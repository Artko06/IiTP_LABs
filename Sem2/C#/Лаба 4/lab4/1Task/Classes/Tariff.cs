using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _1Task.Classes
{
    public class Tariff
    {
        public double costTone { get; private set; }

        public Tariff(double costValue) =>
            costTone = costValue;

        public void UppCost(double value) =>
            costTone += value;

        public void DownCost(double value) =>
            costTone = (costTone > value) ? costTone - value : 0;
    }
}
