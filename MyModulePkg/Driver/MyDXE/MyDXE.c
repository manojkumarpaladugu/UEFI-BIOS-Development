/** @file
  This is a sample UEFI DXE driver.

**/

#include <PiDxe.h>
#include <Library/DebugLib.h>

EFI_STATUS
EFIAPI
MyDXEEntryPoint(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE*    SystemTable
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    DEBUG((DEBUG_INFO, "MyDXEEntryPoint: DXE driver invoked SUCCESSFULLY\n"));

    return Status;
}