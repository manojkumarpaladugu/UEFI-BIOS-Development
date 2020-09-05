/*
  This is a bus driver usage
  This is a sample UEFI application to enumerate all present PCIe devices.

*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>

EFI_STATUS
EFIAPI
MyPCIeEnumerationEntryPoint(
	IN EFI_HANDLE			ImageHandle,
	IN EFI_SYSTEM_TABLE*	SystemTable
)
{
	DEBUG((DEBUG_INFO, "MyPCIeEnumerationEntryPoint: Entry\n"));

	EFI_STATUS				Status = EFI_SUCCESS;
	UINTN					NoHandles = 0;
	EFI_HANDLE*				PciHandles = NULL;
	EFI_PCI_IO_PROTOCOL*	PciIO;
	UINTN                   PciSeg;
	UINTN                   PciBus;
	UINTN                   PciDev;
	UINTN                   PciFun;

	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiPciIoProtocolGuid,
		NULL,
		&NoHandles,
		&PciHandles
	);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "MyPCIeEnumerationEntryPoint: LocateHandleBuffer failed, Status: 0x%x\n", Status));
		goto Exit;
	}

	Print(L"PCI devices count: %d\n", NoHandles);

	for (UINTN HandleIndex = 0; HandleIndex < NoHandles; HandleIndex++) 
	{
		Status = gBS->OpenProtocol(
			PciHandles[HandleIndex],
			&gEfiPciIoProtocolGuid,
			&PciIO,
			ImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
		);
		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "MyPCIeEnumerationEntryPoint: OpenProtocol failed for handle 0x%x, Status: 0x%x\n", PciHandles[HandleIndex], Status));
			continue;
		}

		Status = PciIO->GetLocation(PciIO, &PciSeg, &PciBus, &PciDev, &PciFun);
		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "MyPCIeEnumerationEntryPoint: GetLocation failed, Status: 0x%x\n", Status));
			continue;
		}

		UINT32 VID_DID = 0;
		UINT32 PciVidDidOffset = 0;
		Status = PciIO->Pci.Read(PciIO, EfiPciIoWidthUint32, PciVidDidOffset, 1, &VID_DID);
		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "MyPCIeEnumerationEntryPoint: Pci.Read at Bus 0x%x, Device 0x%x, Function 0x%x, Offset 0x%x failed, Status: 0x%x\n", PciBus, PciDev, PciFun, PciVidDidOffset, Status));
		}
		else {
			Print(L"VID DID: 0x%X\n", VID_DID);
		}
	}

Exit:
	DEBUG((DEBUG_INFO, "MyPCIeEnumerationEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}