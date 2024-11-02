using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _1Task.Classes
{
    public class TrackCompany
    {   
        private static TrackCompany? instance;
        public Tariff tariff { get; private set; }
        public string nameCompany { get; private set; }
        public double toneWeight { get; private set; }

        private TrackCompany(string nameCompany, double toneWeight, double costTariffValue)
        {
            this.nameCompany = nameCompany;
            this.toneWeight = toneWeight;
            tariff = new Tariff(costTariffValue);
        }

        private TrackCompany(string nameCompany)
        {
            this.nameCompany = nameCompany;
            toneWeight = 0;
            tariff = new Tariff(0);
        }

        public static TrackCompany GetInstance(string nameCompany, double toneWeight,
            double costTariffValue) =>
            instance ??= new TrackCompany(nameCompany, toneWeight, costTariffValue);

        public static TrackCompany GetInstance(string nameCompany) =>
            instance ??= new TrackCompany(nameCompany);

        public void UppCost(double value) =>
         tariff.UppCost(value);


        public void DownCost(double value) =>
            tariff.DownCost(value);

        public double profitCompany() =>
            toneWeight * tariff.costTone;
    }
}
