using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.JobResponsibilitiesUseCases.Commands
{
    public sealed class AddJobResponsibilitiesRequest() : IAddOrEditJobResponsibilitiesRequest
    {
        public JobResponsibilities JobResponsibilities { get; set; }
    }

    internal class AddJobResponsibilitiesRequestHandler(IUnitOfWork unitOfWork) : IRequestHandler<AddJobResponsibilitiesRequest>
    {
        public async Task Handle(AddJobResponsibilitiesRequest request, CancellationToken cancellationToken)
        {
            await unitOfWork.JobResponsibilitiesRepository.AddAsync(request.JobResponsibilities, cancellationToken);
            await unitOfWork.SaveAllAsync();
        }
    }
}
