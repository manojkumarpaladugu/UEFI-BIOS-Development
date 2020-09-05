/*

  This is a DXE driver to demonstrate DXE locate protocol interface

*/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/MyProtocol.h>

EFI_STATUS
EFIAPI
MyProtocolDXE2EntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyProtocolDXE2EntryPoint: Entry\n"));
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_GUID MyProtocolGuid = gMyProtocolGuid;
	MY_PROTOCOL_STRUCT* MyProtocolStructPtr = NULL;

	Status = gBS->LocateProtocol(
		&MyProtocolGuid,
		NULL,
		&MyProtocolStructPtr
	);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "LocateProtocol failed, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_INFO, "LocateProtocol success\n"));
		DEBUG((DEBUG_INFO, "Data values is %d\n", MyProtocolStructPtr->Data));
	}

	DEBUG((DEBUG_INFO, "MyProtocolDXE2EntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}