/*

  This is a simple UEFI driver to demonstrate use of wait for event till key press or timer event.

*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
WaitKeyPressOrTimerEventEntryPoint(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable
)
{
  DEBUG((DEBUG_INFO, "WaitKeyPressOrTimerEventEntryPoint: Entry\n"));
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_EVENT WaitList[2];
  UINTN Index;

  //
  // Add ConIn event from the UEFI System Table to the array of events
  //
  WaitList[0] = &gST->ConIn->WaitForKey;

  Status = gBS->CreateEvent(
    EVT_TIMER | EVT_NOTIFY_WAIT,
    TPL_NOTIFY,
    NULL,
    NULL,
    &WaitList[1]
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "CreateEvent failed, Status: 0x%x %r\n", Status, Status));
    goto Exit;
  }

  Status = gBS->SetTimer(
    WaitList[1],
    TimerRelative,
    EFI_TIMER_PERIOD_SECONDS(1)
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "SetTimer failed, Status: 0x%x, %r\n", Status, Status));
    gBS->CloseEvent(WaitList[1]);
    goto Exit;
  }


  //
  // Wait for the console input or the timer to be signaled
  //
  Status = gBS->WaitForEvent(2, WaitList, &Index);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "WaitForEvent failed, Status: 0x%x %r\n", Status, Status));
    gBS->CloseEvent(WaitList[1]);
    goto Exit;
  }

  //
// If the timer event expired return EFI_TIMEOUT
//
  if (!EFI_ERROR(Status) && Index == 1) {
    DEBUG((DEBUG_ERROR, "No key press detected. Timeout.\n"));
    Status = EFI_TIMEOUT;
  }
  
Exit:
  DEBUG((DEBUG_INFO, "WaitKeyPressOrTimerEventEntryPoint: Exit, Status: 0x%x %r\n", Status, Status));
  return Status;
}