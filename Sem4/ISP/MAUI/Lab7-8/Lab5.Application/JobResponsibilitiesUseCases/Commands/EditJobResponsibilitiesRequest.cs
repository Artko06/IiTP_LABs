using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab5.Application.JobResponsibilitiesUseCases.Commands
{
    public sealed class EditJobResponsibilitiesRequest() : IAddOrEditJobResponsibilitiesRequest
    {
        public JobResponsibilities JobResponsibilities { get; set; }
    }

    internal class EditWorkRequestHandler(IUnitOfWork unitOfWork) : IRequestHandler<EditJobResponsibilitiesRequest>
    {
        public async Task Handle(EditJobResponsibilitiesRequest request, CancellationToken cancellationToken)
        {
            await unitOfWork.JobResponsibilitiesRepository.UpdateAsync(request.JobResponsibilities, cancellationToken);
            await unitOfWork.SaveAllAsync();
        }
    }
}
