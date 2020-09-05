//
//
// This is a simple UEFI driver to demonstrate periodic timer event
//
//

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
PeriodicTimerEventNotification(
  IN EFI_EVENT Event,
  IN VOID*     Context
)
{
  DEBUG((DEBUG_INFO, "PeriodicTimerEventNotification called\n"));
}

EFI_STATUS
EFIAPI
PeriodicTimerEventEntryPoint(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable
)
{
  DEBUG((DEBUG_INFO, "PeriodicTimerEventEntryPoint: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;
  EXAMPLE_DEVICE Device;

  Status = gBS->CreateEvent(
    EVT_TIMER | EVT_NOTIFY_SIGNAL,
    TPL_NOTIFY,
    PeriodicTimerEventNotification,
    &Device,
    &Device.PeriodicTimer
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "CreateEvent failed, Status: 0x%x %r\n", Status, Status));
    goto Exit;
  }

  //
  // Program the timer event to be signalled every 100ms.
  //
  Status = gBS->SetTimer(
    Device.PeriodicTimer,
    TimerPeriodic,
    EFI_TIMER_PERIOD_MILLISECONDS(100)
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "SetTimer failed, Status: 0x%x %r\n", Status, Status));
    Status = gBS->CloseEvent(Device.PeriodicTimer);
    goto Exit;
  }
 
Exit:
  DEBUG((DEBUG_INFO, "PeriodicTimerEventEntryPoint: Exit, Status: 0x%x %r\n", Status, Status));
  return Status;
}