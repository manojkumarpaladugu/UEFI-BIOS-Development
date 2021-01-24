#include "MySataOpRom.h"

#define DIAGNOSTIC_LOGBUFFER_MAXCHAR  1024

GLOBAL_REMOVE_IF_UNREFERENCED EFI_DRIVER_DIAGNOSTICS_PROTOCOL gMySataOpRomDriverDiagnostics = {
  (EFI_DRIVER_DIAGNOSTICS_RUN_DIAGNOSTICS) MySataOpRomRunDriverDiagnostics,
  "eng"
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_DRIVER_DIAGNOSTICS2_PROTOCOL gMySataOpRomDriverDiagnostics2 = {
  MySataOpRomRunDriverDiagnostics,
  "en"
};

//
// This diagnostics are for device drivers only
// For device drivers, refer "UEFI Driver Diagnostics->RunDiagnostics() implementation->Device Drivers" in EDK2 writers guide
// EDK2 writers guide: https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/
//
EFI_STATUS
EFIAPI
MySataOpRomRunDriverDiagnostics (
  IN  EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
  IN  EFI_HANDLE                       ControllerHandle,
  IN  EFI_HANDLE                       ChildHandle  OPTIONAL,
  IN  EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType,
  IN  CHAR8                            *Language,
  OUT EFI_GUID                         **ErrorType,
  OUT UINTN                            *BufferSize,
  OUT CHAR16                           **Buffer
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  CHAR8                *SupportedLanguages;
  BOOLEAN              Rfc4646Language;
  BOOLEAN              Found;
  UINTN                Index;
  //EFI_PCI_IO_PROTOCOL  *PciIo;
  //PCI_TYPE00           PciCfg;
  CHAR16               *Message;

  DEBUG ((DEBUG_INFO, "MySataOpRomRunDriverDiagnostics: Entry\n"));

  //
  // ChildHandle must be NULL for a Device Driver
  //
  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }

  if ((ControllerHandle == NULL) ||
      (Language         == NULL) ||
      (ErrorType        == NULL) ||
      (BufferSize       == NULL) ||
      (Buffer           == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Make sure this driver is currently managing ControllerHandle
  //
  Status = EfiTestManagedDevice(
              ControllerHandle,
              gMySataOpRomDriverBinding.DriverBindingHandle,
              &gEfiPciIoProtocolGuid
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Make sure Language is in the set of Supported Languages
  //
  SupportedLanguages = This->SupportedLanguages;
  Rfc4646Language = (BOOLEAN)(This == &gMySataOpRomDriverDiagnostics2);
  Found = FALSE;
  while (*SupportedLanguages != 0) {
    if (Rfc4646Language) {
      for (Index = 0; SupportedLanguages[Index] != 0 && SupportedLanguages[Index] != ';'; Index++);
      if ((AsciiStrnCmp(SupportedLanguages, Language, Index) == 0) && (Language[Index] == 0)) {
        Found = TRUE;
        break;
      }
      SupportedLanguages += Index;
      for (; *SupportedLanguages != 0 && *SupportedLanguages == ';'; SupportedLanguages++);
    }
    else {
      if (CompareMem(Language, SupportedLanguages, 3) == 0) {
        Found = TRUE;
        break;
      }
      SupportedLanguages += 3;
    }
  }
  //
  // If Language is not a member of SupportedLanguages, then return EFI_UNSUPPORTED
  //
  if (!Found) {
    return EFI_UNSUPPORTED;
  }

  //
  // Perform Diagnostics Algorithm on ControllerHandle for the
  // type of diagnostics requested in DiagnosticsType
  //
  // Return results in ErrorType, Buffer, and BufferSize
  //
  // If Diagnostics Algorithm fails, then return EFI_DEVICE_ERROR
  //
  *ErrorType = NULL;
  Message = L"Running Driver Diagnostics\n";
  *BufferSize = DIAGNOSTIC_LOGBUFFER_MAXCHAR;
  *Buffer = AllocateZeroPool(DIAGNOSTIC_LOGBUFFER_MAXCHAR);
  StrCpyS (*Buffer, *BufferSize, Message);
  DEBUG((DEBUG_INFO, "DIAG MESSAGE: %s, Size: %d\n", *Buffer, *BufferSize));

  DEBUG((DEBUG_INFO, "MySataOpRomRunDriverDiagnostics: Exit. Status: 0x%x, %r\n", Status, Status));
  return Status;
}


//
// This below diagnostics are for bus drivers only
// For bus drivers, refer "UEFI Driver Diagnostics->RunDiagnostics() implementation->Bus Drivers and Hybrid Drivers" in EDK2 writers guide
// EDK2 writers guide: https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/
//
/*
EFI_STATUS
EFIAPI
MySataOpRomRunDriverDiagnostics(
  IN  EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
  IN  EFI_HANDLE                       ControllerHandle,
  IN  EFI_HANDLE                       ChildHandle  OPTIONAL,
  IN  EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType,
  IN  CHAR8                            *Language,
  OUT EFI_GUID                         **ErrorType,
  OUT UINTN                            *BufferSize,
  OUT CHAR16                           **Buffer
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  CHAR8      *SupportedLanguages;
  BOOLEAN    Rfc4646Language;
  BOOLEAN    Found;
  UINTN      Index;

  if ((ControllerHandle == NULL) ||
    (Language == NULL) ||
    (ErrorType == NULL) ||
    (BufferSize == NULL) ||
    (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Make sure this driver is currently managing ControllerHandle
  //
  Status = EfiTestManagedDevice (
              ControllerHandle,
              gMySataOpRomDriverBinding.DriverBindingHandle,
              &gEfiPciIoProtocolGuid
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // If ChildHandle is not NULL, then make sure this driver produced ChildHandle
  //
  if (ChildHandle != NULL) {
    Status - EfiTestManagedDevice(
                ControllerHandle,
                &ChildHandle,
                &gEfiPciIoProtocolGuid
                );
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  //
  // Make sure Language is in the set of Supported Languages
  //
  SupportedLanguages = This->SupportedLanguages;
  Rfc4646Language = (BOOLEAN)(This == &gMySataOpRomDriverDiagnostics2);
  Found = FALSE;
  while (*SupportedLanguages != 0) {
    if (Rfc4646Language) {
      for (Index = 0; SupportedLanguages[Index] != 0 && SupportedLanguages[Index] != ';'; Index++);
      if ((AsciiStrnCmp(SupportedLanguages, Language, Index) == 0) && (Language[Index] == 0)) {
        Found = TRUE;
        break;
      }
      SupportedLanguages += Index;
      for (; *SupportedLanguages != 0 && *SupportedLanguages == ';'; SupportedLanguages++);
    }
    else {
      if (CompareMem(Language, SupportedLanguages, 3) == 0) {
        Found = TRUE;
        break;
      }
      SupportedLanguages += 3;
    }
  }
  //
  // If Language is not a member of SupportedLanguages, then return EFI_UNSUPPORTED
  //
  if (!Found) {
    return EFI_UNSUPPORTED;
  }

  if (ChildHandle == NULL) {
    //
    // Perform Diagnostics Algorithm on the bus controller specified
    // by ControllerHandle for the type of diagnostics requested in
    // DiagnosticsType
    //
    // Return results in ErrorType, Buffer, and BufferSize
    //
    // If Diagnostics Algorithm fails, then return EFI_DEVICE_ERROR
    //
  }
  else {
    //
    // Perform Diagnostics Algorithm on child controller specified
    // by ChildHandle for the type of diagnostics requested in
    // DiagnosticsType
    //
    // Return results in ErrorType, Buffer, and BufferSize
    //
    // If Diagnostics Algorithm fails, then return EFI_DEVICE_ERROR
    //
  }

  return Status;
}
*/