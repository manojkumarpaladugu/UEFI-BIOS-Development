//
//
//  This is a simple UEFI driver to find total PCI controllers.
//
//

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/PciRootBridgeIo.h>

EFI_STATUS
EFIAPI
CountPCIControllersEntryPoint(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  DEBUG((DEBUG_INFO, "CountPCIControllersEntryPoint - Entry\n"));

  EFI_STATUS                          Status           = EFI_SUCCESS;
  UINTN                               HandleCount      = 0;
  EFI_HANDLE                          *HandleBuffer     = NULL;
  EFI_HANDLE                          ControllerHandle = NULL;
  EFI_OPEN_PROTOCOL_INFORMATION_ENTRY *EntryBuffer;
  UINTN                               Index;
  UINTN                               EntryCount       = 0;
  UINTN                               NumberOfChildren = 0;
  UINT32                              Attributes;

  Status = gBS->LocateHandleBuffer(
    ByProtocol,
    &gEfiPciRootBridgeIoProtocolGuid,
    NULL,
    &HandleCount,
    &HandleBuffer
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "LocateHandleBuffer for gEfiPciRootBridgeIoProtocolGuid failed, Status: 0x%X, %r\n",Status, Status));
    goto Exit;
  }

  ControllerHandle = HandleBuffer[0];
  FreePool(HandleBuffer);

  Status = gBS->OpenProtocolInformation(
    ControllerHandle,
    &gEfiPciRootBridgeIoProtocolGuid,
    &EntryBuffer,
    &EntryCount
  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "OpenProtocolInformation for gEfiPciRootBridgeIoProtocolGuid failed, Status: 0x%X, %r\n", Status, Status));
    goto Exit;
  }

  //
  // Count the number child handles that are currently using the PCI Root
  // Bridge I/O Protocol on ControllerHandle children
  //
  for (Index = 0, NumberOfChildren = 0; Index < EntryCount; Index++) {
    Attributes = EntryBuffer[Index].Attributes;
    if ((Attributes & EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER) != 0) {
      NumberOfChildren++;
    }
  }

  //
  // Free the buffer allocated by OpenProtocolInformation()
  //
  FreePool(EntryBuffer);

  Print(L"Total PCI Controllers: %u\n", NumberOfChildren);

Exit:
  DEBUG((DEBUG_INFO, "CountPCIControllersEntryPoint - Exit, Status: 0x%x, %r\n", Status, Status));
  return Status;
}