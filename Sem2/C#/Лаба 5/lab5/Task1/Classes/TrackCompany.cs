using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using Task1.Classes;
using static Task1.Classes.Tariff;

namespace Task1.Classes
{
    internal class TrackCompany
    {
        private static TrackCompany? instance;
        public string nameCompany { get; private set; }
        private readonly List<Client> listOfClients;

        private TrackCompany(string nameCompany)
        {
            this.nameCompany = nameCompany;
            listOfClients = new List<Client>();
        }

        public void AddClient(string nicknameClient, string passwordClient)
        {
            bool clientExists = false;

            for (int i = 0; i < listOfClients.Count; ++i)
            {
                if (nicknameClient == listOfClients.ElementAt(i).nicknameClient)
                {
                    Console.WriteLine("The client already exists");
                    clientExists = true;
                }
            }

            if (!clientExists)
            {
                listOfClients.Add(new Client(nicknameClient, passwordClient));
                Console.WriteLine("The client was successfully added");
            }
        }

        public void OrderTransportation(string nicknameClient, string passwordClient,
            double loadWeightKg, Tariff.Tariffs tariff)
        {
            for (int i = 0; i < listOfClients.Count; ++i)
            {
                if (nicknameClient == listOfClients.ElementAt(i).nicknameClient)
                {
                    if (passwordClient == listOfClients.ElementAt(i).passwordClient)
                    {
                        Console.WriteLine("Order sum: " + CalculateSumOrder(loadWeightKg, tariff));

                        listOfClients.ElementAt(i).totalSumOrders += CalculateSumOrder(loadWeightKg,
                            tariff);

                        Console.WriteLine("Successfully");
                        break;
                    }

                    else
                    {
                        Console.WriteLine("Wrong password");
                        break;
                    }
                }

                else if (i == listOfClients.Count - 1)
                {
                    Console.WriteLine("The client was not found");
                }
            }
        }

        private static double CalculateSumOrder(double loadWeightKg, Tariff.Tariffs tariff) =>
            loadWeightKg * ((byte)tariff);

        public void GetTotalSumOrders(string nicknameClient, string passwordClient)
        {
            for (int i = 0; i < listOfClients.Count; ++i)
            {
                if (nicknameClient == listOfClients.ElementAt(i).nicknameClient)
                {
                    if (passwordClient == listOfClients.ElementAt(i).passwordClient)
                    {
                        Console.WriteLine(listOfClients.ElementAt(i).totalSumOrders);
                        break;
                    }

                    else
                    {
                        Console.WriteLine("Wrong password");
                        break;
                    }
                }

                else if (i == listOfClients.Count - 1)
                {
                    Console.WriteLine("Wrong nickname");
                    break;
                }
            }
        }

        public void GetAllClients()
        {
            for (int i = 0; i < listOfClients.Count; ++i)
            {
                Console.WriteLine($"{i + 1}. " + listOfClients.ElementAt(i).nicknameClient);
            }
        }

        public static TrackCompany GetInstance(string nameCompany) =>
            instance ??= new TrackCompany(nameCompany);
    }
}