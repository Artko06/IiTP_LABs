using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace _353503_KOKHAN_LAB8.Entities
{
    public class CarShowroom(int id, string name, int countWhereCapacityEngineMoreTwoLitres)
    {
        public int Id { get; set; } = id;
        public string CarName { get; set; } = name;
        public int CountWhereCapacityEngineMoreTwoLitres { get; set; } = countWhereCapacityEngineMoreTwoLitres;

        // Пустой конструктор (обязателен для десериализации)
        public CarShowroom() : this(0, "", 100)
        {

        }
    }
}