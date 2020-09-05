/*

  This is a UEFI driver to demonstrate UEFI Service Binding Protocol.

*/

#include "Driver.h"

EFI_HANDLE gAbcServiceBindingHandle = NULL;

EFI_SERVICE_BINDING_PROTOCOL gAbcServiceBindingProtocol = {
  AbcServiceBindingCreateChild,
  AbcServiceBindingDestroyChild
};

typedef struct {
  UINT32 AbcField;
} ABC_PROTOCOL;

ABC_PROTOCOL gAbc = {
  25
};

EFI_STATUS
EFIAPI
AbcServiceBindingCreateChild(
  IN     EFI_SERVICE_BINDING_PROTOCOL *This,
  IN OUT EFI_HANDLE                   *ChildHandle
) 
{
  DEBUG((DEBUG_INFO, "AbcServiceBindingCreateChild: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;

  Status = gBS->InstallMultipleProtocolInterfaces(
    ChildHandle,
    &gEfiAbcProtocolGuid,
    &gAbc,
    NULL
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "gEfiAbcProtocolGuid, install failed\n"));
  }
  else {
    DEBUG((DEBUG_INFO, "gEfiAbcProtocolGuid, install success\n"));
  }

  DEBUG((DEBUG_INFO, "AbcServiceBindingCreateChild: Exit, ChildHandle: 0x%X, Status: %r\n",ChildHandle, Status));
  return Status;
}

EFI_STATUS
EFIAPI
AbcServiceBindingDestroyChild(
  IN EFI_SERVICE_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   ChildHandle
) 
{
  DEBUG((DEBUG_INFO, "AbcServiceBindingDestroyChild: Entry, ChildHandle: 0x%X\n", ChildHandle));
  EFI_STATUS Status = EFI_SUCCESS;

  Status = gBS->UninstallMultipleProtocolInterfaces(
    ChildHandle,
    &gEfiAbcProtocolGuid,
    &gAbc,
    NULL
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "gEfiAbcProtocolGuid, uninstall failed\n"));
  }
  else {
    DEBUG((DEBUG_INFO, "gEfiAbcProtocolGuid, uninstall success\n"));
  }

  DEBUG((DEBUG_INFO, "AbcServiceBindingDestroyChild: Exit, Status: %r\n", Status));
  return Status;
}

EFI_STATUS
EFIAPI
MyServiceDriverEntryPoint(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  DEBUG((DEBUG_INFO, "MyServiceDriverEntryPoint: Entry\n"));

  EFI_STATUS Status = EFI_SUCCESS;

  // Install the Service Binding Protocol
  Status = gBS->InstallMultipleProtocolInterfaces(
    &gAbcServiceBindingHandle,
    &gEfiAbcServiceBindingProtocolGuid,
    &gAbcServiceBindingProtocol
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "gEfiAbcServiceBindingProtocolGuid, install protocol failed.\n"));
  }
  else {
    DEBUG((DEBUG_INFO, "gEfiAbcServiceBindingProtocolGuid, install protocol success.\n"));
  }

  DEBUG((DEBUG_INFO, "MyServiceDriverEntryPoint: Exit, Status: %r\n", Status));
  return Status;
}