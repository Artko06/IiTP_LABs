using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.JobResponsibilitiesUseCases.Commands
{
    public interface IAddOrEditJobResponsibilitiesRequest : IRequest
    {
        JobResponsibilities JobResponsibilities { get; set; }
    }
}
