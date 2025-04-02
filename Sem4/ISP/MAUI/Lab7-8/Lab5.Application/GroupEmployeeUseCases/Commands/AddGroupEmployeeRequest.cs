using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.GroupEmployeeUseCases.Commands
{
    public sealed class AddGroupEmployeeRequest : IAddOrEditGroupEmployeeRequest
    {
        public GroupEmployee GroupEmployee { get; set; }
    }

    internal class AddGroupEmployeeRequestHandler(IUnitOfWork unitOfWork) : IRequestHandler<AddGroupEmployeeRequest>
    {
        public async Task Handle(AddGroupEmployeeRequest request, CancellationToken cancellationToken)
        {
            await unitOfWork.GroupEmployeeRepository.AddAsync(request.GroupEmployee, cancellationToken);
            await unitOfWork.SaveAllAsync();
        }
    }
}
