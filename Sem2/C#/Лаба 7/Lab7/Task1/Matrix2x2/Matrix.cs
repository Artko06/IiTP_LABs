using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Matrix2x2
{
    internal class Matrix
    {
        private int[,] matrix = new int[2, 2];

        public int this[int i, int j]
        {
            get
            {
                if (i < 0 || i > 1 || j < 0 || j > 1)
                {
                    throw new IndexOutOfRangeException("Индекс вне диапазона матрицы 2х2\n");
                }
                return matrix[i, j];
            }
            set
            {
                if (i < 0 || i > 1 || j < 0 || j > 1)
                {
                    throw new IndexOutOfRangeException("Индекс вне диапазона матрицы 2х2\n");
                }
                matrix[i, j] = value;
            }
        }

        public Matrix(){}

        public Matrix(int a11, int a12, int a21, int a22)
        {
            matrix[0, 0] = a11;
            matrix[0, 1] = a12;
            matrix[1, 0] = a21;
            matrix[1, 1] = a22;
        }

        public static Matrix operator +(Matrix firstMatrix, Matrix secondMatrix)
        {
            Matrix result = new Matrix();

            for (byte i = 0; i < 2; ++i)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    result[i, j] = firstMatrix[i, j] + secondMatrix[i, j];
                }
            }
            return result;
        }

        public static Matrix operator ++(Matrix value)
        {
            Matrix result = new Matrix();

            for (byte i = 0; i < 2; ++i)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    result[i, j] = ++value[i, j];
                }
            }
            return result;
        }

        public static Matrix operator -(Matrix firstMatrix, Matrix secondMatrix)
        {
            Matrix result = new Matrix();

            for (byte i = 0; i < 2; ++i)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    result[i, j] = firstMatrix[i, j] - secondMatrix[i, j];
                }
            }
            return result;
        }

        public static Matrix operator --(Matrix valueMatrix)
        {
            for (byte i = 0; i < 2; ++i)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    --valueMatrix[i, j];
                }
            }
            return valueMatrix;
        }

        public static Matrix operator *(Matrix firstMatrix, Matrix secondMatrix)
        {
            Matrix result = new Matrix();

            result[0, 0] = firstMatrix[0, 0] * secondMatrix[0, 0] + firstMatrix[0, 1] * secondMatrix[1, 0];
            result[0, 1] = firstMatrix[0, 0] * secondMatrix[0, 1] + firstMatrix[0, 1] * secondMatrix[1, 1];
            result[1, 0] = firstMatrix[1, 0] * secondMatrix[0, 0] + firstMatrix[1, 1] * secondMatrix[1, 0];
            result[1, 1] = firstMatrix[1, 0] * secondMatrix[0, 1] + firstMatrix[1, 1] * secondMatrix[1, 1];
            return result;
        }

        public static Matrix operator *(Matrix valueMatrix, int value)
        {
            for (byte i = 0; i < 2; ++i)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    valueMatrix[i, j] *= value;
                }
            }
            return valueMatrix;
        }

        public static Matrix operator /(Matrix valueMatrix, int value)
        {
            for (byte i = 0; i < 2; ++i)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    valueMatrix[i, j] /= value;
                }
            }
            return valueMatrix;
        }

        public static bool operator ==(Matrix firstMatrix, Matrix secondMatrix)
        {
            for (byte i = 0; i < 2; ++i)
            {
                for (byte j = 0; j < 2; ++j)
                {
                    if (firstMatrix[i, j] != secondMatrix[i, j])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        public static bool operator >(Matrix firstMatrix, Matrix secondMatrix)
        {
            if(firstMatrix[0, 0] * firstMatrix[1, 1] - firstMatrix[0, 1] * firstMatrix[1, 0] >
                secondMatrix[0, 0] * secondMatrix[1, 1] - secondMatrix[0, 1] * secondMatrix[1, 0])
            {
                return true;
            }
                return false;
        }

        public static bool operator <(Matrix firstMatrix, Matrix secondMatrix)
        {
            if (firstMatrix[0, 0] * firstMatrix[1, 1] - firstMatrix[0, 1] * firstMatrix[1, 0] <
                secondMatrix[0, 0] * secondMatrix[1, 1] - secondMatrix[0, 1] * secondMatrix[1, 0])
            {
                return true;
            }
            return false;
        }

        public static bool operator !=(Matrix firstMatrix, Matrix secondMatrix) =>
            !(firstMatrix == secondMatrix);

        public static bool operator true(Matrix valueMatrix) =>
           valueMatrix[0, 0] * valueMatrix[1, 1] - valueMatrix[0, 1] * valueMatrix[1, 0] != 0;

        public static bool operator false(Matrix valueMatrix) =>
            valueMatrix[0, 0] * valueMatrix[1, 1] - valueMatrix[0, 1] * valueMatrix[1, 0] == 0;

        public static explicit operator int(Matrix valueMatrix) =>
            valueMatrix[0, 0] * valueMatrix[1, 1] - valueMatrix[0, 1] * valueMatrix[1, 0];

        public static explicit operator Matrix(int a) =>
            new Matrix { [0, 0] = a, [0, 1] = 0, [1, 0] = 0, [1, 1] = a };

        public override string ToString() =>
            $"{matrix[0, 0]} {matrix[0, 1]}\n{matrix[1, 0]} {matrix[1, 1]}\n";

    }
}