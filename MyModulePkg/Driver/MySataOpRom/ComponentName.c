#include "MySataOpRom.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE mMySataOpRomDriverNameTable[] = {
  { "eng:en", L"My SATA Option ROM Driver in English" },
  { "fra;fr", L"My SATA Option ROM Driver in French" },
  { NULL, NULL }
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE mMySataOpRomControllerNameTable[] = {
  {"eng:en", L"My SATA Controller in English" },
  {"fra:fr", L"My SATA Controller in French" },
  { NULL, NULL }
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME_PROTOCOL gMySataOpRomComponentName = {
  (EFI_COMPONENT_NAME_GET_DRIVER_NAME)GetDriverName,
  (EFI_COMPONENT_NAME_GET_CONTROLLER_NAME)GetControllerName,
  "engfra"
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME2_PROTOCOL gMySataOpRomComponentName2 = {
  GetDriverName,
  GetControllerName,
  "en;fr"
};

EFI_STATUS
EFIAPI
GetDriverName(
  IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
)
{
  return LookupUnicodeString2(
            Language,
            This->SupportedLanguages,
            mMySataOpRomDriverNameTable,
            DriverName,
            (BOOLEAN)(This != &gMySataOpRomComponentName2)
            );
}

EFI_STATUS
EFIAPI
GetControllerName(
  IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  EFI_HANDLE                   ChildHandle        OPTIONAL,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **ControllerName
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //
  // ChildHandle must be NULL for a device driver
  //
  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Make sure this driver is currently managing the ControllerHandle
  //
  Status = EfiTestManagedDevice(
              ControllerHandle,
              gMySataOpRomDriverBinding.DriverBindingHandle,
              &gEfiPciIoProtocolGuid
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return LookupUnicodeString2(
            Language,
            This->SupportedLanguages,
            mMySataOpRomControllerNameTable,
            ControllerName,
            (BOOLEAN)(This != &gMySataOpRomComponentName2)
            );
}