/*

  This is a simple UEFI driver to demnonstrate on how to use SetVariable() and GetVariable() services.

*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "ExampleConfigurationVariable.h"

EFI_STATUS
EFIAPI
WriteReadUefiVariableEntryPoint(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable)
{
  DEBUG((DEBUG_INFO, "WriteReadUefiVariableEntryPoint: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 Attributes;
  EXAMPLE_CONFIGURATION ExampleConfigurationForWrite = { 11, 12, 13 };
  EXAMPLE_CONFIGURATION ExampleConfigurationForRead;
  UINTN DataSize;

  Status = gRT->SetVariable(
    EXAMPLE_CONFIGURATION_VARIABLE_NAME,
    &gExampleConfigurationVariableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof(ExampleConfigurationForWrite),
    &ExampleConfigurationForWrite
    );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "SetVariable failed, Status: 0x%x %r\n", Status, Status));
    goto Exit;
  }

  DataSize = sizeof(ExampleConfigurationForRead);
  Attributes = EFI_VARIABLE_NON_VOLATILE |
    EFI_VARIABLE_BOOTSERVICE_ACCESS |
    EFI_VARIABLE_RUNTIME_ACCESS;

  Status = gRT->GetVariable(
    EXAMPLE_CONFIGURATION_VARIABLE_NAME,
    &gExampleConfigurationVariableGuid,
    &Attributes,
    &DataSize,
    &ExampleConfigurationForRead
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "GetVariable failed, Status: 0x%x %r\n", Status, Status));
    goto Exit;
  }

  DEBUG((DEBUG_INFO, "Question1: %d, Question2: %d, Question3: %d\n",
    ExampleConfigurationForRead.Question1,
    ExampleConfigurationForRead.Question2,
    ExampleConfigurationForRead.Question3));

Exit:
  DEBUG((DEBUG_INFO, "WriteReadUefiVariableEntryPoint: Exit, Status: 0x%x %r\n", Status, Status));
  return Status;
}