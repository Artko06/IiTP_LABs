using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.JobResponsibilitiesUseCases.Queries
{
    public sealed record GetByGroupJobResponsibilitiesRequest(int Id) : IRequest<IEnumerable<JobResponsibilities>>;

    internal class GetByGroupJobResponsibilitiesRequestHandler(IUnitOfWork unitOfWork) :
        IRequestHandler<GetByGroupJobResponsibilitiesRequest, IEnumerable<JobResponsibilities>>
    {
        public async Task<IEnumerable<JobResponsibilities>> Handle(
            GetByGroupJobResponsibilitiesRequest request,
            CancellationToken cancellationToken)
        {
            return await unitOfWork.JobResponsibilitiesRepository.ListAsync(x => x.GroupEmployeeId == request.Id);
        }
    }
}
