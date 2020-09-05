/*

  This is a simple UEFI driver to get UEFI variable storage info.

*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

EFI_STATUS
EFIAPI
GetUefiVariableStorageInfoEntryPoint(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable
)
{
  DEBUG((DEBUG_INFO, "GetUefiVariableStorageInfoEntryPoint: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64 MaximumVariableStorageSize;
  UINT64 RemainingVariableStorageSize;
  UINT64 MaximumVariableSize;

  Status = gRT->QueryVariableInfo(
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    &MaximumVariableStorageSize,
    &RemainingVariableStorageSize,
    &MaximumVariableSize
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "QueryVariableInfo failed, Status: 0x%x %r\n", Status, Status));
    goto Exit;
  }

  DEBUG((DEBUG_INFO, "MaximumVariableStorageSize: %llu\n \
RemainingVariableStorageSize: %llu\n \
MaximumVariableSize: %llu\n", 
MaximumVariableStorageSize, 
RemainingVariableStorageSize, 
MaximumVariableSize));

Exit:
  DEBUG((DEBUG_INFO, "GetUefiVariableStorageInfoEntryPoint: Exit, Status: 0x%x %r\n", Status, Status));
  return Status;
}