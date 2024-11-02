using Task1.Matrix2x2;

namespace Task1
{
    file class Program
    {
        static void Main(string[] args)
        {
            Matrix matrix1 = new Matrix(1, 2, 3, 4);
            Matrix matrix2 = new Matrix(5, 6, 7, 8);

            Console.WriteLine("Матрица 1:");
            Console.WriteLine(matrix1.ToString());

            Console.WriteLine("Матрица 2:");
            Console.WriteLine(matrix2.ToString());

            Matrix sum = matrix1 + matrix2;
            Console.WriteLine("Сумма матриц 1 и 2:");
            Console.WriteLine(sum.ToString());

            Matrix diff = matrix1 - matrix2;
            Console.WriteLine("Разность матриц 1 и 2:");
            Console.WriteLine(diff.ToString());

            Matrix product = matrix1 * matrix2;
            Console.WriteLine("Произведение матриц 1 и 2:");
            Console.WriteLine(product.ToString());

            Matrix increment = ++matrix1;
            Console.WriteLine("Инкремент матрицы 1:");
            Console.WriteLine(increment.ToString());

            Matrix decrement = --matrix2;
            Console.WriteLine("Декремент матрицы 2:");
            Console.WriteLine(decrement.ToString());

            Matrix multiplyByNumber = matrix1 * 2;
            Console.WriteLine("Умножение матрицы 1 на число 2:");
            Console.WriteLine(multiplyByNumber.ToString());

            Matrix divideByNumber = matrix1 / 2;
            Console.WriteLine("Деление матрицы 1 на число 2:");
            Console.WriteLine(divideByNumber.ToString());

            bool areEqual = matrix1 == matrix2;
            Console.WriteLine("Матрицы 1 и 2 равны: " + areEqual);

            bool areNotEqual = matrix1 != matrix2;
            Console.WriteLine("Матрицы 1 и 2 не равны: " + areNotEqual);

            bool isFirstGreater = matrix1 > matrix2;
            Console.WriteLine("Матрица 1 больше матрицы 2: " + isFirstGreater);

            bool isFirstLess = matrix1 < matrix2;
            Console.WriteLine("Матрица 1 меньше матрицы 2: " + isFirstLess);

            if(matrix1)
            {
                Console.WriteLine("Определитель матрицы 1 НЕ равен 0");
            }

            else
            {
                Console.WriteLine("Определитель матрицы 1 равен 0");
            }

            int determinant1 = (int)matrix1;
            Console.WriteLine("Определитель матрицы 1: " + determinant1);

            int determinant2 = (int)matrix2;
            Console.WriteLine("Определитель матрицы 2: " + determinant2);

            Matrix fromNumber = (Matrix)determinant1;
            Console.WriteLine("Матрица из числа (определителя матрицы 1):");
            Console.WriteLine(fromNumber.ToString());

            //Console.WriteLine($"Попробуем обратится к индексу [2,3]: {matrix1[2, 3]}");
        }
    }
}