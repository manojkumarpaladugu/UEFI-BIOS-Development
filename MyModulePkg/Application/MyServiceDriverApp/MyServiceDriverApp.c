/*

  This is a sample UEFI application to consume service binding protocol

*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DebugLib.h>
#include <Protocol/ServiceBinding.h>
#include <Library/UefiBootServicesTableLib.h>

typedef struct {
  UINT32 AbcField;
} ABC_PROTOCOL;

EFI_STATUS
EFIAPI
MyServiceDriverAppEntryPoint(
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS                    Status                      = EFI_SUCCESS;
  EFI_HANDLE                    AbcHandle                   = NULL;
  EFI_HANDLE                    ChildHandle                 = NULL;
  EFI_SERVICE_BINDING_PROTOCOL  *mAbcServiceBindingProtocol = NULL;
  UINTN                         NumHandles;
  EFI_HANDLE                    *HandleBuffer;
  ABC_PROTOCOL                  *AbcProtocolStruct;

  //
   // Open a passive ABC Service Binding instance
   //

  Status = gBS->LocateHandleBuffer(
    ByProtocol,
    &gEfiAbcServiceBindingProtocolGuid,
    NULL,
    &NumHandles,
    &HandleBuffer
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR: Find ABC Service Binding: %r\n", Status));
    return Status;
  }

  // We just take the first handle
  AbcHandle = HandleBuffer[0];

  Status = gBS->OpenProtocol(
    AbcHandle,
    &gEfiAbcServiceBindingProtocolGuid,
    (VOID**)&mAbcServiceBindingProtocol,
    gImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR: Open ABC Service Binding: %r\n", Status));
    return Status;
  }

  Status = mAbcServiceBindingProtocol->CreateChild(mAbcServiceBindingProtocol, &ChildHandle);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR: ABC ServiceBinding Create: %r\n", Status));
    return Status;
  }

  Status = gBS->LocateProtocol(
    &gEfiAbcProtocolGuid,
    NULL,
    &AbcProtocolStruct
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR: LocateProtocol failed, Status: 0x%x\n", Status));
    return Status;
  }

  Print(L"AbcField value is %d\n", AbcProtocolStruct->AbcField);

  Status = mAbcServiceBindingProtocol->DestroyChild(mAbcServiceBindingProtocol, ChildHandle);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "ERROR: ABC ServiceBinding Destroy: %r\n", Status));
    return Status;
  }

  return Status;
}