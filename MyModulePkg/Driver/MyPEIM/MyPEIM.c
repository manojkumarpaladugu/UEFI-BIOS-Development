/** @file
  This is a sample UEFI PEI driver.

**/

#include <PiPei.h>
#include <Library/DebugLib.h>

EFI_STATUS
EFIAPI
MyPEIMEntryPoint(
    IN EFI_PEI_FILE_HANDLE      FileHandle,
    IN CONST EFI_PEI_SERVICES** PeiServices
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    DEBUG((DEBUG_INFO, "MyPEIMEntryPoint: PEI driver invoked SUCCESSFULLY\n"));

    return Status;
}