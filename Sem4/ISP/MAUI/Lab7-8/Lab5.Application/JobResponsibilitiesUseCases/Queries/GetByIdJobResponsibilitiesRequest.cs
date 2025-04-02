using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.JobResponsibilitiesUseCases.Queries
{
    public sealed record GetByIdJobResponsibilitiesRequest(int Id) : IRequest<JobResponsibilities?>;

    internal class GetJobResponsibilitiesByGroupRequestHandler(IUnitOfWork unitOfWork) : 
        IRequestHandler<GetByIdJobResponsibilitiesRequest, JobResponsibilities?>
    {
        public async Task<JobResponsibilities?> Handle(GetByIdJobResponsibilitiesRequest request, CancellationToken cancellationToken)
        {
            return await unitOfWork.JobResponsibilitiesRepository.GetByIdAsync(request.Id, cancellationToken);
        }
    }
}
