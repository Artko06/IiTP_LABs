namespace _353503_KOKHAN_LAB3.Entities
{
    public static class Journal
    {   
        private static List<string> listOfEvents = new List<string>();

        public static void LogEvent(Client client)
        {   
            string message = $"The client {client.Name} was added";
            Console.WriteLine(message + "\n");
            listOfEvents.Add(message);
        }

        public static void OutputAllEvents()
        {
            Console.WriteLine("Events: ");
            for (int i = 0; i < listOfEvents.Count; ++i)
            {
                Console.WriteLine($"{i+1}. {listOfEvents[i]}");
            }
            Console.WriteLine();
        }
    }
}