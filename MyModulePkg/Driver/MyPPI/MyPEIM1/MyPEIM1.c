/** @file
  This is a sample UEFI PEI driver to install a PPI.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Ppi/MyPPI.h>

PeiPpiServiceData gPpi = { 55 };
EFI_PEI_PPI_DESCRIPTOR gPpiDescriptor = { EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST, &MyPEIM1PpiGuid, (VOID*)&gPpi };

EFI_STATUS
EFIAPI
MyPEIM1EntryPoint(
    IN EFI_PEI_FILE_HANDLE      FileHandle,
    IN CONST EFI_PEI_SERVICES** PeiServices
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    DEBUG((DEBUG_INFO, "MyPEIM1EntryPoint: Entry\n"));

    Status = PeiServicesInstallPpi(&gPpiDescriptor);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "MyPEIM1EntryPoint: PeiServicesInstallPpi failed, Status: 0x%x\n", Status));
    }
    else
    {
        DEBUG((DEBUG_INFO, "MyPEIM1EntryPoint: PeiServicesInstallPpi, Status: %r\n", Status));
    }

    DEBUG((DEBUG_INFO, "MyPEIM1EntryPoint: Exit\n"));
    return Status;
}