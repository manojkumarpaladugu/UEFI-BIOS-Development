//
//
// This is a simple to demonstrate event generated on ExitBootServices() call.
//
//

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

VOID
EFIAPI
NotifyExitBootServices(
  IN EFI_EVENT Event,
  IN VOID      *Context
)
{
  DEBUG((DEBUG_INFO, "NotifyExitBootServices called\n"));
}

EFI_STATUS
EFIAPI
EvtExitBootServicesEntryPoint(
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  DEBUG((DEBUG_INFO, "EvtExitBootServicesEntryPoint: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;

  EFI_EVENT ExitBootServicesEvent = NULL;
  Status = gBS->CreateEvent(
    EVT_SIGNAL_EXIT_BOOT_SERVICES,
    TPL_NOTIFY,
    NotifyExitBootServices,
    NULL,
    &ExitBootServicesEvent
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "CreateEvent EVT_SIGNAL_EXIT_BOOT_SERVICES failed, Status: 0x%x, %r\n", Status, Status));
  }

  DEBUG((DEBUG_INFO, "EvtExitBootServicesEntryPoint: Exit, Status: 0x%x, %r\n", Status, Status));
  return Status;
}