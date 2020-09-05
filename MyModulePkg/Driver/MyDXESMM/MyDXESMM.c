/*

  This is a simple DXE SMM driver to register SMI handler.

*/

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Library/DebugLib.h>

#define SMI_Input_Value 0xE0

EFI_STATUS
EFIAPI
SmiHandler(
	EFI_HANDLE      DispatchHandle,
	IN  CONST VOID* DispatchContext OPTIONAL,
	IN  OUT VOID*   CommBuffer      OPTIONAL,
	IN  OUT UINTN*  CommBufferSize  OPTIONAL
)
{
	DEBUG((DEBUG_INFO, "SmiHandler: In SMM Handler\n"));
	return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MyDXESMMEntryPoint(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyDXESMMEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;

	EFI_HANDLE  SmmSwHandle = NULL;
	EFI_SMM_SW_DISPATCH2_PROTOCOL* SmmSwDispatch = NULL;
	EFI_SMM_SW_REGISTER_CONTEXT    SmmSwContext;
	EFI_SMM_BASE2_PROTOCOL* SmmBasePtr = NULL;
	EFI_SMM_SYSTEM_TABLE2* SmstPtr;

	Status = gBS->LocateProtocol(
		&gEfiSmmBase2ProtocolGuid,
		NULL,
		&SmmBasePtr);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "LocateProtocol failed, Status: 0x%x\n", Status));
		goto Exit;
	}

	Status = SmmBasePtr->GetSmstLocation(SmmBasePtr, &SmstPtr);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "GetSmstLocation failed, Status: 0x%x\n", Status));
		goto Exit;
	}

	Status = SmstPtr->SmmLocateProtocol(
		&gEfiSmmSwDispatch2ProtocolGuid,
		NULL,
		&SmmSwDispatch);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "SmmLocateProtocol failed, Status: 0x%x\n", Status));
		goto Exit;
	}

	SmmSwContext.SwSmiInputValue = SMI_Input_Value;
	Status = SmmSwDispatch->Register(SmmSwDispatch,
		SmiHandler,
		&SmmSwContext,
		&SmmSwHandle);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "Register failed, Status: 0x%x\n", Status));
		goto Exit;
	}

	DEBUG((DEBUG_INFO, "SMI handler successfully registered\n"));

Exit:
	DEBUG((DEBUG_INFO, "MyDXESMMEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}