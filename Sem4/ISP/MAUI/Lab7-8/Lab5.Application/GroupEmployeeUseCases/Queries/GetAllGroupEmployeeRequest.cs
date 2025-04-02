using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.GroupEmployeeUseCases.Queries
{
    public sealed record GetAllGroupEmployeeRequest() : IRequest<IEnumerable<GroupEmployee>>;

    internal class GetAllBrigadesRequestHandler(IUnitOfWork unitOfWork) : IRequestHandler<GetAllGroupEmployeeRequest, IEnumerable<GroupEmployee>>
    {
        public async Task<IEnumerable<GroupEmployee>> Handle(
            GetAllGroupEmployeeRequest request,
            CancellationToken cancellationToken)
        {
            return await unitOfWork.GroupEmployeeRepository.ListAllAsync(cancellationToken);
        }
    }
}
