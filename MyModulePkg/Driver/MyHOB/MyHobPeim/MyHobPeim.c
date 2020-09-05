/**
  This is a sample UEFI PEI driver to demonstrate HOB creation.
**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <MyHob.h>

EFI_STATUS
EFIAPI
MyHobPeimEntryPoint(
	IN EFI_PEI_FILE_HANDLE FileHandle,
	IN CONST EFI_PEI_SERVICES** PeiServices
)
{
	DEBUG((DEBUG_INFO, "MyHobPeimEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;
	STRUCT_HOB* HobStructPointer = NULL;

	PeiServicesCreateHob(
		EFI_HOB_TYPE_GUID_EXTENSION,
		sizeof(STRUCT_HOB),
		&HobStructPointer
	);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "MyHobPeimEntryPoint: PeiServicesCreateHob failed, Status: 0x%x\n", Status));
	}
	else
	{
		HobStructPointer->EfiHobGuidType.Name = gMyHobGuid;
		HobStructPointer->data = 10;
		DEBUG((DEBUG_INFO, "MyHobPeimEntryPoint: PeiServicesCreateHob success\n"));
	}

	DEBUG((DEBUG_INFO, "MyHobPeimEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}