using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.JobResponsibilitiesUseCases.Queries
{
    public sealed record GetAllJobResponsibilitiesRequest() : IRequest<IEnumerable<JobResponsibilities>>;

    internal class GetAllJobResponsibilitiesRequestHandler(IUnitOfWork unitOfWork) : IRequestHandler<GetAllJobResponsibilitiesRequest,
        IEnumerable<JobResponsibilities>>
    {
        public async Task<IEnumerable<JobResponsibilities>> Handle(
            GetAllJobResponsibilitiesRequest request, 
            CancellationToken cancellationToken)
        {
            return await unitOfWork.JobResponsibilitiesRepository.ListAllAsync();
        }
    }
}
