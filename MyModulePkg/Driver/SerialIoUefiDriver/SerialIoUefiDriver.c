/*

  This is a DXE driver to demonstrate UEFI driver model.

*/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/SerialIo.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
SerialIoSupported(
	IN EFI_DRIVER_BINDING_PROTOCOL*		This,
	IN EFI_HANDLE						ControllerHandle,
	IN EFI_DEVICE_PATH_PROTOCOL*		RemainingDevicePath OPTIONAL
)
{
	DEBUG((DEBUG_INFO, "SerialIoSupported: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;
	EFI_SERIAL_IO_PROTOCOL* SerialIo = NULL;

	Status = gBS->OpenProtocol(
		ControllerHandle,
		&gEfiSerialIoProtocolGuid,
		&SerialIo,
		This->DriverBindingHandle,
		ControllerHandle,
		EFI_OPEN_PROTOCOL_BY_DRIVER
	);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "SerialIoSupported: OpenProtocol failed, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_INFO, "SerialIoSupported: OpenProtocol success\n"));
	}

	Status = gBS->CloseProtocol(
		ControllerHandle,
		&gEfiSerialIoProtocolGuid,
		This->DriverBindingHandle,
		ControllerHandle
	);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "SerialIoSupported: CloseProtocol failed, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_INFO, "SerialIoSupported: CloseProtocol success\n"));
	}

	DEBUG((DEBUG_INFO, "SerialIoSupported: Exit, Status: 0x%x\n", Status));
	return Status;
}

EFI_STATUS
EFIAPI
SerialIoStart(
	IN EFI_DRIVER_BINDING_PROTOCOL*		This,
	IN EFI_HANDLE						ControllerHandle,
	IN EFI_DEVICE_PATH_PROTOCOL*		RemainingDevicePath OPTIONAL
)
{
	DEBUG((DEBUG_INFO, "SerialIoStart: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;
	EFI_SERIAL_IO_PROTOCOL* SerialIo = NULL;

	Status = gBS->OpenProtocol(
		ControllerHandle,
		&gEfiSerialIoProtocolGuid,
		&SerialIo,
		This->DriverBindingHandle,
		ControllerHandle,
		EFI_OPEN_PROTOCOL_BY_DRIVER
	);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "SerialIoStart: OpenProtocol failed, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_INFO, "SerialIoStart: OpenProtocol success\n"));
		goto Exit;
	}

	UINT8 String[] = "SerialIoStart method (Using write function call)\n";
	UINTN Size = sizeof(String);
	Status = SerialIo->Write(SerialIo, &Size, String);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "SerialIoStart: Write failed, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_INFO, "SerialIoStart: Write success\n"));
	}

Exit:
	DEBUG((DEBUG_INFO, "SerialIoStart: Exit, Status: 0x%x\n", Status));
	return Status;
}

EFI_STATUS
EFIAPI
SeiralIoStop(
	IN EFI_DRIVER_BINDING_PROTOCOL* This,
	IN EFI_HANDLE                            ControllerHandle,
	IN UINTN                                 NumberOfChildren,
	IN EFI_HANDLE* ChildHandleBuffer OPTIONAL
)
{
	DEBUG((DEBUG_INFO, "SeiralIoStop: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;

	Status = gBS->CloseProtocol(
		ControllerHandle,
		&gEfiSerialIoProtocolGuid,
		This->DriverBindingHandle,
		ControllerHandle
		);
	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "SeiralIoStop: CloseProtocol failed, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_INFO, "SeiralIoStop: CloseProtocol success\n"));
	}

	DEBUG((DEBUG_INFO, "SeiralIoStop: Exit, Status: 0x%x\n", Status));
	return Status;
}

EFI_DRIVER_BINDING_PROTOCOL SerialIoDriverBindingProtocol = {
										SerialIoSupported,
                                        SerialIoStart,
                                        SeiralIoStop,
                                        0x10, //Version
                                        NULL, //Image Handle
                                        NULL //DriverBindingHandle
};

EFI_STATUS
EFIAPI
SerialIoUefiDriverEntryPoint(
	IN EFI_HANDLE			ImageHandle,
	IN EFI_SYSTEM_TABLE*	SystemTable
)
{
	DEBUG((DEBUG_INFO, "SerialIoUefiDriverEntryPoint: Entry\n"));

	EFI_STATUS Status = EFI_SUCCESS;

	SerialIoDriverBindingProtocol.ImageHandle = ImageHandle;
	SerialIoDriverBindingProtocol.DriverBindingHandle = ImageHandle;

	Status = gBS->InstallMultipleProtocolInterfaces(
		SerialIoDriverBindingProtocol.DriverBindingHandle,
		&gEfiDriverBindingProtocolGuid,
		&SerialIoDriverBindingProtocol,
		NULL
	);

	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "SerialIoUefiDriverEntryPoint: Failed to install Serial IO driver, Status: 0x%x\n", Status));
	}
	else {
		DEBUG((DEBUG_ERROR, "SerialIoUefiDriverEntryPoint: Serial IO driver successfully installed\n"));
	}

	DEBUG((DEBUG_INFO, "SerialIoUefiDriverEntryPoint: Exit, Status: 0x%x\n", Status));
	return Status;
}