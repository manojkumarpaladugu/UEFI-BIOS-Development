//
//
// This is a sample UEFI application to demonstrate Events.
//
//

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

VOID
EFIAPI
ExampleEventNotification(
  IN EFI_EVENT* Event,
  IN VOID*      Context
)
{
  Print(L"ExampleEventNotification called\n");
}

EFI_STATUS
EFIAPI
ExampleEventEntryPoint(
  IN EFI_HANDLE    ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable
)
{
  DEBUG((DEBUG_INFO, "ExampleEventEntryPoint: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_EVENT ExampleEvent;

  Status = gBS->CreateEventEx(
    EVT_NOTIFY_SIGNAL,
    TPL_NOTIFY,
    ExampleEventNotification,
    NULL,
    &gExampleEventGroupGuid,
    &ExampleEvent
    );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "CreateEventEx gExampleEventGroupGuid failed, Status: 0x%x, %r\n", Status, Status));
    goto Exit;
  }

  //
  // Signal the event causing all notification functions for this
  // event group to be executed
  //
  Status = gBS->SignalEvent(ExampleEvent);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "SignalEvent failed, Status: 0x%x, %r\n", Status, Status));
    goto Exit;
  }

  //
  // Close the event
  //
  Status = gBS->CloseEvent(ExampleEvent);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "CloseEvent failed, Status: 0x%x, %r\n", Status, Status));
    goto Exit;
  }

Exit:
  DEBUG((DEBUG_INFO, "ExampleEventEntryPoint: Exit, Status: 0x%x, %r\n", Status, Status));
  return Status;
}