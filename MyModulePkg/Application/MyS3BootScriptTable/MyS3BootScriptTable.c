/*

  This is a simple UEFI application for accessing S3 boot script table.

*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PiDxeS3BootScriptLib/InternalBootScriptLib.h>

#define _PCD_TOKEN_PcdS3BootScriptTablePrivateDataPtr               319U
#define _PCD_GET_MODE_64_PcdS3BootScriptTablePrivateDataPtr         LibPcdGet64(_PCD_TOKEN_PcdS3BootScriptTablePrivateDataPtr)

EFI_STATUS
EFIAPI
MyS3BootScriptTableEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyS3BootScriptTableEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;

	SCRIPT_TABLE_PRIVATE_DATA* S3BootScriptTablePtr;
	S3BootScriptTablePtr = (SCRIPT_TABLE_PRIVATE_DATA*)(UINTN)PcdGet64(PcdS3BootScriptTablePrivateDataPtr);
	Print(L"Table length=%d", S3BootScriptTablePtr->TableLength);

	DEBUG((DEBUG_INFO, "MyS3BootScriptTableEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}