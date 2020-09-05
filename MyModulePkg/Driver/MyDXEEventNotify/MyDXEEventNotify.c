/*
  This is a simple DXE driver to demonstrate Event Notification.
*/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Guid/EventGroup.h>

void EventNotifyFunction(
	IN EFI_EVENT Event,
	IN VOID*     Context
)
{
	DEBUG((DEBUG_INFO, "EventNotifyFunction: Event Notify triggered\n"));
}

EFI_STATUS
EFIAPI
MyDXEEventNotifyEntryPoint(
	IN EFI_HANDLE        ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyDXEEventNotifyEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;

	EFI_EVENT Event;
	//EFI_GUID gEfiEventReadyToBootGuid = EFI_EVENT_GROUP_READY_TO_BOOT;
	EFI_EVENT_NOTIFY EventNotifyFnPtr = EventNotifyFunction;

	Status = SystemTable->BootServices->CreateEventEx(
		EVT_NOTIFY_SIGNAL,
		TPL_NOTIFY,
		EventNotifyFnPtr,
		NULL,
		&gEfiEventReadyToBootGuid,
		&Event
	);

	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_INFO, "MyDXEEventNotifyEntryPoint: CreateEventEx failed, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_INFO, "MyDXEEventNotifyEntryPoint: CreateEventEx success\n"));
	}

	DEBUG((DEBUG_INFO, "MyDXEEventNotifyEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}