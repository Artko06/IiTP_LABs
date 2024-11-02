namespace Task1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            bool replay = true;

            while (replay)
            {
                string? str = null;
                long number = 0;

            while (true)
            {
                Console.WriteLine("Input integer number:");
                str = Console.ReadLine();

                if (long.TryParse(str, out number))
                {
                    number = long.Parse(str);
                    break;
                }
            }

                Console.WriteLine("Number later: " + MyMath.Operation(number));

                Console.WriteLine("Input 'e' to end or another symbol to continue");
                str = Console.ReadLine();

                if (str == "e") replay = false;

            }
        }
    }
}