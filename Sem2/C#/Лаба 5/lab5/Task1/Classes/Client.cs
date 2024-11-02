using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal class Client
    {
        public string nicknameClient { get; private set; }
        public string passwordClient { get; private set; }
        public double totalSumOrders { get; set; } = 0;

        public Client(string nicknameClient, string passwordClient)
        {
            this.nicknameClient = nicknameClient;
            this.passwordClient = passwordClient;
        }
    }
}