/*

  This is an example of device driver usage
  This is a simple UEFI application to read SATA using Block IO protocol.

*/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/BlockIo.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS
EFIAPI
MySataBlockIoReaderEntryPoint(
	IN EFI_HANDLE			ImageHandle,
	IN EFI_SYSTEM_TABLE*	SystemTable
)
{
	Print(L"MySataBlockIoReaderEntryPoint: Entry\n");

	EFI_STATUS Status = EFI_SUCCESS;
	UINTN NoHandles = 0;
	EFI_HANDLE* BlockIoHandles = NULL;
	EFI_DEVICE_PATH_PROTOCOL* PartitionDevicePath = NULL;
	EFI_BLOCK_IO_PROTOCOL* BlockIo = NULL;

	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiBlockIoProtocolGuid,
		NULL,
		&NoHandles,
		&BlockIoHandles
	);
	if (EFI_ERROR(Status)) {
		Print(L"MySataBlockIoReaderEntryPoint: LocateHandleBuffer failed, Status: 0x%x\n", Status);
		goto Exit;
	}

	Print(L"Block IO handles count: %d\n", NoHandles);

	for (UINTN HandleIndex = 0; HandleIndex < NoHandles; HandleIndex++)
	{
		Status = gBS->OpenProtocol(
			BlockIoHandles[HandleIndex],
			&gEfiDevicePathProtocolGuid,
			&PartitionDevicePath,
			ImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
		);
		if (EFI_ERROR(Status)) {
			Print(L"MySataBlockIoReaderEntryPoint: OpenProtocol failed for handle 0x%x, Status: 0x%x\n", BlockIoHandles[HandleIndex], Status);
			continue;
		}

		Print(L"Type: 0x%2x, Sub Type: 0x%2x\n", PartitionDevicePath->Type, PartitionDevicePath->SubType);

		if (!((PartitionDevicePath->Type == MEDIA_DEVICE_PATH) && (PartitionDevicePath->SubType == MEDIA_HARDDRIVE_DP))) {
			continue;
		}

		Status = gBS->OpenProtocol(
			BlockIoHandles[HandleIndex],
			&gEfiBlockIoProtocolGuid,
			&BlockIo,
			ImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
		);
		if (EFI_ERROR(Status)) {
			Print(L"MySataBlockIoReaderEntryPoint: OpenProtocol failed for handle 0x%x, Status: 0x%x\n", BlockIoHandles[HandleIndex], Status);
			continue;
		}

		UINT8* Buffer = AllocatePool(BlockIo->Media->BlockSize);
		Status = BlockIo->ReadBlocks(
			BlockIo,
			BlockIo->Media->MediaId,
			0,
			BlockIo->Media->BlockSize,
			Buffer
		);
		if (EFI_ERROR(Status)) {
			Print(L"MySataBlockIoReaderEntryPoint: ReadBlocks failed for media id 0x%x, Status: 0x%x\n", BlockIo->Media->MediaId, Status);
			continue;
		}

		Print(L"Block data for Media ID: 0x%x, Block Size: %d\n", BlockIo->Media->MediaId, BlockIo->Media->BlockSize);
		
		Print(L"0x");
		for (UINT32 Index = 0; Index < BlockIo->Media->BlockSize; Index++)
		{
			Print(L"%2x", Buffer[Index]);
		}
		Print(L"\n\n");
	}

Exit:
	Print(L"MySataBlockIoReaderEntryPoint: Exit, Status: 0x%x\n", Status);
	return Status;
}