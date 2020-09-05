/** @file
  This is a sample UEFI PEI driver to consume a PPI.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Ppi/MyPPI.h>

EFI_STATUS
EFIAPI
MyPEIM2EntryPoint(
    IN EFI_PEI_FILE_HANDLE      FileHandle,
    IN CONST EFI_PEI_SERVICES** PeiServices
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    DEBUG((DEBUG_INFO, "MyPEIM2EntryPoint: Entry\n"));
    PeiPpiServiceData* Ppi = NULL;

    EFI_GUID temp = MyPEIM1PpiGuid;
    DEBUG((DEBUG_INFO, "MyPEIM1PpiGuid: %8X-%4X-%4X-%2X%2X%2X%2X%2X%2X%2X%2X\n", 
        temp.Data1, 
        temp.Data2, 
        temp.Data3, 
        temp.Data4[0], temp.Data4[1], temp.Data4[2], temp.Data4[3], temp.Data4[4], temp.Data4[5], temp.Data4[6], temp.Data4[7]));

    Status = PeiServicesLocatePpi(&MyPEIM1PpiGuid, 0, NULL, (VOID**)&Ppi);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_INFO, "MyPEIM2EntryPoint: PeiServicesLocatePpi failed, Status: 0x%x\n", Status));
    }
    else
    {
        DEBUG((DEBUG_INFO, "MyPEIM2EntryPoint: PeiServicesLocatePpi Successful, Data: %d\n", Ppi->Data));
    }

    DEBUG((DEBUG_INFO, "MyPEIM2EntryPoint: Exit\n"));
    return Status;
}