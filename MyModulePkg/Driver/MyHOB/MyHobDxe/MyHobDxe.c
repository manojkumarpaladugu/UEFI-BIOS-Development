/**
  This is a sample UEFI PEI driver to consume HOB.
**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <MyHob.h>

EFI_STATUS
EFIAPI
MyHobDxeEntryPoint(
	IN EFI_HANDLE        ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyHobDxeEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;
	STRUCT_HOB* HobStructPointer = NULL;
	HobStructPointer = GetFirstGuidHob(&gMyHobGuid);
	if (HobStructPointer == NULL) {
		DEBUG((DEBUG_ERROR, "Unable to find HOB\n"));
	}
	else {
		DEBUG((DEBUG_INFO, "HOB data is %d\n", HobStructPointer->data));
	}

	DEBUG((DEBUG_INFO, "MyHobDxeEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}