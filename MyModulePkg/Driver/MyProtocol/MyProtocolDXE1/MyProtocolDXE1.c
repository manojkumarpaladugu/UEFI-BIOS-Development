/*

  This is a DXE driver to demonstrate DXE install protocol interface

*/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/MyProtocol.h>

EFI_STATUS
EFIAPI
MyProtocolDXE1EntryPoint(
	IN EFI_HANDLE        ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyProtocolDXE1EntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;
	EFI_GUID MyProtocolGuid = gMyProtocolGuid;
	MY_PROTOCOL_STRUCT* MyProtocolStructPtr = (MY_PROTOCOL_STRUCT*)AllocatePool(sizeof(MY_PROTOCOL_STRUCT));
	MyProtocolStructPtr->Data = 25;

	Status = SystemTable->BootServices->InstallProtocolInterface(
		&ImageHandle,
		&MyProtocolGuid,
		EFI_NATIVE_INTERFACE,
		MyProtocolStructPtr
	);

	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "MyProtocolDXE1EntryPoint: InstallProtocolInterface failed, Status 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_ERROR, "MyProtocolDXE1EntryPoint: InstallProtocolInterface success\n"));
	}

	DEBUG((DEBUG_INFO, "MyProtocolDXE1EntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}