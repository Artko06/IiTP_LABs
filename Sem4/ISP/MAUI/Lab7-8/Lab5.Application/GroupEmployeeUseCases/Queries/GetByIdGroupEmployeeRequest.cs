using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.GroupEmployeeUseCases.Queries
{
    public sealed record GetByIdGroupEmployeeRequest(int Id) : IRequest<GroupEmployee?>;

    internal class GetByIdGroupEmployeeRequestHandler(IUnitOfWork unitOfWork) : IRequestHandler<GetByIdGroupEmployeeRequest, GroupEmployee?>
    {
        public async Task<GroupEmployee?> Handle(GetByIdGroupEmployeeRequest request, CancellationToken cancellationToken)
        {
            return await unitOfWork.GroupEmployeeRepository.GetByIdAsync(request.Id, cancellationToken);
        }
    }
}
