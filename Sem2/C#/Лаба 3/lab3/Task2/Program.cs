namespace Task2
{
    internal class Program
    {   
        static void CorrectInput(out double value)
        {   
            string? str = null;

            while (true)
            {
                str = Console.ReadLine();

                if (double.TryParse(str, out value))
                {
                    value = double.Parse(str);
                    break;
                }
                Console.WriteLine("re-Input value: ");
            }
        }

        static void Main(string[] args)
        {
            bool replay = true;

            while (replay)
            {
                string? str = null;
                double z = 0, b = 0;

                    Console.WriteLine("Input z:");
                CorrectInput(out z);

                    Console.WriteLine("Input b:");
                CorrectInput(out b);

                Console.WriteLine((b == 0 && z < 1)? "Indefinite" : 
                    "y =  " + Services.MyMath.Foo(z, b));

                Console.WriteLine("Input 'e' to end or another symbol to continue");
                str = Console.ReadLine();

                if (str == "e") replay = false;

            }
        }
    }
}