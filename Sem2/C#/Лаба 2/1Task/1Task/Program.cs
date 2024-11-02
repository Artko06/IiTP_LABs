namespace _1Task
{
    internal class Program
    {
        static void Main(string[] args)
        {
            bool replay = true;

            while (replay)
            {
                string str = "";
                int number = 0;
                bool convert = false;

                while (!convert)
                {
                    Console.WriteLine("Input a two-digit number:");
                    str = Console.ReadLine();

                    convert = int.TryParse(str, out number);

                    if (convert)
                    {
                        if (Math.Abs(number) < 10 || Math.Abs(number) > 99) convert = false;

                        number = int.Parse(str);
                    }
                }

                number = Math.Abs(number % 10 + number / 10);

                Console.WriteLine("The sum of the digits = " + number + "\n");
                if (number % 3 == 0)
                {
                    Console.WriteLine("Answer: True");
                }

                else Console.WriteLine("Answer: False");

                Console.WriteLine("Input '+' to continue or another symbol to end\n");
                str = Console.ReadLine();

                if (str != "+") replay = false;

            }

        }
    }
}