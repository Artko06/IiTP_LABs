namespace Task2
{
    internal class Program
    {
        static void Main(string[] args)
        {
            bool replay = true;

            while (replay)
            {
                string str = "";
                double x = 0, y = 0;
                byte caseNumber = 0;
                bool convert = false;

                while (!convert)
                {
                    Console.WriteLine("Input x:");
                    str = Console.ReadLine();

                    convert = double.TryParse(str, out x);

                    if (convert)
                    {
                        x = double.Parse(str);
                    }
                }

                convert = false;

                while (!convert)
                {
                    Console.WriteLine("Input y:");
                    str = Console.ReadLine();

                    convert = double.TryParse(str, out y);

                    if (convert)
                    {
                        y = double.Parse(str);
                    }
                }

                if(x == -y || y == 23 || x == 0)
                {
                    caseNumber = 1;
                }

                else if(y > 23 || x > 0 || x + y < 0)
                {
                    caseNumber = 2;
                }

                switch(caseNumber)
                {
                    case 0:
                        {
                            Console.WriteLine("Inside");
                            break;
                        }
                    case 1:
                        {
                            Console.WriteLine("At the border");
                            break;
                        }
                    case 2:
                        {
                            Console.WriteLine("Outside");
                            break;
                        }
                }

                Console.WriteLine("Input '+' to continue or another symbol to end:");
                str = Console.ReadLine();

                if (str != "+") replay = false;

            }
        }
    }
}