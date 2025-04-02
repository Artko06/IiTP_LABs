using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.GroupEmployeeUseCases.Commands
{
    public interface IAddOrEditGroupEmployeeRequest : IRequest
    {
        GroupEmployee GroupEmployee { get; set; }
    }
}
