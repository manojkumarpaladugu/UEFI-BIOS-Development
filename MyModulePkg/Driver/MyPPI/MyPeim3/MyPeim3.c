/** @file
  This is a sample UEFI PEI driver to consume a PPI MyPEIM1PpiGuid.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Ppi/MyPPI.h>

EFI_STATUS
EFIAPI
MyPeim3EntryPoint (
  IN       EFI_PEI_FILE_HANDLE FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
)
{
  EFI_STATUS        Status = EFI_SUCCESS;
  PeiPpiServiceData *Ppi   = NULL;

  DEBUG ((DEBUG_INFO, "MyPeim3EntryPoint - Entry\n"));

  Status = PeiServicesLocatePpi (&MyPEIM1PpiGuid, 0, NULL, (VOID**)&Ppi);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "MyPeim3EntryPoint: PeiServicesLocatePpi failed\n"));
  } else {
    DEBUG((DEBUG_INFO, "MyPeim3EntryPoint: PeiServicesLocatePpi Successful, Data: %d\n", Ppi->Data));
  }

  DEBUG((DEBUG_INFO, "MyPeim3EntryPoint - Exit, Status: %r\n", Status));
  return Status;
}