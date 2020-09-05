/*

  This is a simple UEFI application to generate SMI.

*/

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>

#define Port 0xB2

// This is the input value registered for SMI handle in MyDXESMM driver
#define SMI_Input_Value 0xE0

EFI_STATUS
EFIAPI
MyDXESMMAppEntryPoint(
	IN EFI_HANDLE        ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyDXESMMAppEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;

	IoWrite8(Port, SMI_Input_Value);

	DEBUG((DEBUG_INFO, "MyDXESMMAppEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}