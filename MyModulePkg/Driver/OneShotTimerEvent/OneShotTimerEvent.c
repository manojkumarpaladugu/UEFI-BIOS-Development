/*

  This example demonstrates CheckEvent()

  CheckEvent()
    This service checks to see if an event is in the waiting state or the signaled state. EFI Drivers creating 
    events use this service to determine when an event has been signaled with SignalEvent(). Such events include 
    timer events, those used to determine when input is available, or events associated with non-blocking I/O operations.

    The example below is an example that creates a one-shot timer event signaled 4 seconds in the future. CheckEvent() is 
    called in a loop waiting for the timer event to be signaled.
*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

typedef struct {
  UINTN Signature;
  EFI_EVENT PeriodicTimer;
  EFI_EVENT OneShotTimer;
  //
  // Other device specific fields
  //
} EXAMPLE_DEVICE;

VOID
EFIAPI
OneShotTimerEventNotification(
  IN EFI_EVENT Event,
  IN VOID*     Context
)
{
  DEBUG((DEBUG_INFO, "OneShotTimerEventNotification called\n"));
}

EFI_STATUS
EFIAPI
OneShotTimerEventEntryPoint(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable
)
{
  DEBUG((DEBUG_INFO, "OneShotTimerEventEntryPoint: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;
  EXAMPLE_DEVICE Device;

  Status = gBS->CreateEvent(
    EVT_TIMER | EVT_NOTIFY_SIGNAL,
    TPL_NOTIFY,
    OneShotTimerEventNotification,
    NULL,
    &Device.OneShotTimer
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "CreateEvent failed, Status: 0x%x %r\n", Status, Status));
    goto Exit;
  }

  Status = gBS->SetTimer(
    Device.OneShotTimer,
    TimerRelative,
    EFI_TIMER_PERIOD_SECONDS(4)
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "SetTimer failed, Status: 0x%x %r\n", Status, Status));
    Status = gBS->CloseEvent(Device.OneShotTimer);
    goto Exit;
  }

Exit:
  DEBUG((DEBUG_INFO, "OneShotTimerEventEntryPoint: Exit, Status: 0x%x %r\n", Status, Status));
  return Status;
}