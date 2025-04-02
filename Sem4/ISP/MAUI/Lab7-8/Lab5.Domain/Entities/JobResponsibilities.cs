using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Domain.Entities
{
    // Класс, описывающий обязанности сотрудника
    public class JobResponsibilities : Entity
    {
        public int GroupEmployeeId { get; set; }

        // Название обязанности
        public string ResponsibilityName { get; set; }

        // Важность обязанности по 10-бальной шкале
        public int Importance { get; set; }

        // Статус выполнения обязанности
        public string Status { get; set; }

        // Срок выполнения обязанности
        public DateTime Deadline { get; set; }
    }
}
