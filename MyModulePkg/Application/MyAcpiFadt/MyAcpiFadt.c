/*

  This is a sample UEFI application to print FADT contents.

*/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Acpi20.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/Acpi.h>

EFI_STATUS
EFIAPI
MyAcpiFadtEntryPoint(
	IN EFI_HANDLE			ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyAcpiFadtEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;
	EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER* RSDP = NULL;
	EFI_ACPI_DESCRIPTION_HEADER* RSDTP = NULL;
	EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE* FADTP = NULL;

	for (UINTN Index = 0; Index < gST->NumberOfTableEntries; Index++)
	{
		if (CompareGuid(&(gST->ConfigurationTable[Index].VendorGuid), &gEfiAcpi20TableGuid)) {
			RSDP = gST->ConfigurationTable[Index].VendorTable;
			break;
		}
	}

	if (RSDP == NULL) {
		DEBUG((DEBUG_ERROR, "MyAcpiFadtEntryPoint:  RSDP is null\n"));
		goto Exit;
	}

	RSDTP = (EFI_ACPI_DESCRIPTION_HEADER*)(UINTN)RSDP->RsdtAddress;
	if (RSDTP == NULL || RSDTP->Signature != EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
		DEBUG((DEBUG_ERROR, "MyAcpiFadtEntryPoint: RSDTP is null or signature not matching\n"));
		goto Exit;
	}

	for (UINT32 Index = sizeof(EFI_ACPI_DESCRIPTION_HEADER); Index < RSDTP->Length; Index += sizeof(UINT32)) {
		FADTP = (EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE*)(UINTN)*(UINT32*)((UINT8*)RSDTP + Index);
		if (FADTP->Header.Signature == EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
			break;
		}
	}

	if (FADTP == NULL || FADTP->Header.Signature != EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
		DEBUG((DEBUG_ERROR, "MyAcpiFadtEntryPoint: FADTP is null or signature not matching\n"));
		goto Exit;
	}

	Print(L"Flags =%d\n DayAlrm=%d\n Century=%d\n", FADTP->Flags, FADTP->DayAlrm, FADTP->Century);

Exit:
	DEBUG((DEBUG_INFO, "MyAcpiFadtEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}