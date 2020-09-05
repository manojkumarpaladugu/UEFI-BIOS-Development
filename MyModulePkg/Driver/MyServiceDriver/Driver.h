/*

  This is a UEFI driver to demonstrate UEFI Service Binding Protocol.

*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Protocol/ServiceBinding.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
AbcServiceBindingCreateChild(
  IN     EFI_SERVICE_BINDING_PROTOCOL* This,
  IN OUT EFI_HANDLE* ChildHandle
);

EFI_STATUS
EFIAPI
AbcServiceBindingDestroyChild(
  IN EFI_SERVICE_BINDING_PROTOCOL* This,
  IN EFI_HANDLE                   ChildHandle
);