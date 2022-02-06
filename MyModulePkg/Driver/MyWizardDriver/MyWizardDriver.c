/** @file
  TODO: Brief Description of UEFI Driver MyWizardDriver
  
  TODO: Detailed Description of UEFI Driver MyWizardDriver

  TODO: Copyright for UEFI Driver MyWizardDriver
  
  TODO: License for UEFI Driver MyWizardDriver

**/

#include "MyWizardDriver.h"
//HII support
EFI_GUID           mMyWizardDriverFormSetGuid = MYWIZARDDRIVER_FORMSET_GUID;
CHAR16             mIfrVariableName[]         = L"MWD_IfrNVData";
EFI_HANDLE         mDriverHandle[2]           = { NULL, NULL };
MYWIZARDDRIVER_DEV *PrivateData               = NULL;

// HII support for Device Path
HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    MYWIZARDDRIVER_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

#define  DUMMY_SIZE 100*16		// Dummy buffer
CHAR16	*DummyBufferfromStart = NULL;

/**
  Unloads an image.

  @param  ImageHandle           Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.

**/
EFI_STATUS 
EFIAPI
MyWizardDriverUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  DEBUG((DEBUG_INFO, "MyWizardDriverUnload Entry\n"));

  //
  // Release resources allocated by the entry point.
  //
  HiiRemovePackages(PrivateData->HiiHandle[0]);
  gBS->UninstallMultipleProtocolInterfaces(
    ImageHandle,
    &gEfiDevicePathProtocolGuid,
    &mHiiVendorDevicePath,
    &gEfiHiiConfigAccessProtocolGuid,
    &PrivateData->ConfigAccess,
    NULL
  );

  DEBUG((DEBUG_INFO, "MyWizardDriverUnload Exit, Status: %r\n", Status));
  return Status;
}

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
MyWizardDriverDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  DEBUG((DEBUG_INFO, "MyWizardDriverDriverEntryPoint: Entry\n"));

  EFI_STATUS  Status;
  // HII Locals
  EFI_HII_DATABASE_PROTOCOL        *HiiDatabase;
  EFI_HII_HANDLE                   HiiHandle[2];
  EFI_HII_STRING_PROTOCOL          *HiiString;
  EFI_FORM_BROWSER2_PROTOCOL       *FormBrowser2;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  UINTN                            BufferSize;
  MYWIZARDDRIVER_CONFIGURATION     *Configuration;
  BOOLEAN                          ActionFlag;
  EFI_STRING                       ConfigRequestHdr;

  Status = EFI_SUCCESS;

  // Initialize driver private data
  PrivateData = AllocateZeroPool(sizeof(MYWIZARDDRIVER_DEV));
  if (PrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PrivateData->Signature = MYWIZARDDRIVER_DEV_SIGNATURE;
  PrivateData->ConfigAccess = gMyWizardDriverHiiConfigAccess;

  //
  // Locate Hii Database protocol
  //
  Status = gBS->LocateProtocol(&gEfiHiiDatabaseProtocolGuid, NULL, (VOID**)&HiiDatabase);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  PrivateData->HiiDatabase = HiiDatabase;

  //
  // Locate HiiString protocol
  //
  Status = gBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID**)&HiiString);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  PrivateData->HiiString = HiiString;

  //
  // Locate Formbrowser2 protocol
  //
  Status = gBS->LocateProtocol(&gEfiFormBrowser2ProtocolGuid, NULL, (VOID**)&FormBrowser2);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  PrivateData->FormBrowser2 = FormBrowser2;

  //
  // Locate ConfigRouting protocol
  //
  Status = gBS->LocateProtocol(&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID**)&HiiConfigRouting);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  PrivateData->HiiConfigRouting = HiiConfigRouting;

  //
  // Publish sample Fromset and config access 
  //
  DEBUG((DEBUG_INFO, "Installing gEfiHiiConfigAccessProtocolGuid\n"));
  Status = gBS->InstallMultipleProtocolInterfaces(
    &mDriverHandle[0],
    &gEfiDevicePathProtocolGuid,
    &mHiiVendorDevicePath,
    &gEfiHiiConfigAccessProtocolGuid,
    &PrivateData->ConfigAccess,
    NULL
  );
  ASSERT_EFI_ERROR(Status);

  PrivateData->DriverHandle[0] = mDriverHandle[0];

  //
  // Publish our HII data
  //
  DEBUG((DEBUG_INFO, "HiiAddPackages\n"));
  HiiHandle[0] = HiiAddPackages(
    &mMyWizardDriverFormSetGuid,
    mDriverHandle[0],
    MyWizardDriverStrings,
    MyWizardDriverFormsBin,
    NULL
  );
  if (HiiHandle[0] == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PrivateData->HiiHandle[0] = HiiHandle[0];

  //
  // Initialize configuration data
  //
  Configuration = &PrivateData->Configuration;
  ZeroMem(Configuration, sizeof(MYWIZARDDRIVER_CONFIGURATION));

  //
  // Try to read NV config EFI variable first
  //
  ConfigRequestHdr = HiiConstructConfigHdr(&mMyWizardDriverFormSetGuid, mIfrVariableName, mDriverHandle[0]);
  ASSERT(ConfigRequestHdr != NULL);
  DEBUG((DEBUG_INFO, "ConfigRequestHdr: %s\n", ConfigRequestHdr));

  DEBUG((DEBUG_INFO, "GetVariable - %s\n", mIfrVariableName));
  BufferSize = sizeof(MYWIZARDDRIVER_CONFIGURATION);
  Status = gRT->GetVariable(
    mIfrVariableName,
    &mMyWizardDriverFormSetGuid,
    NULL,
    &BufferSize,
    Configuration
  );
  if (EFI_ERROR(Status)) {  // Not definded yet so add it to the NV Variables.
    // zero out buffer
    ZeroMem(Configuration, sizeof(MYWIZARDDRIVER_CONFIGURATION));
    DEBUG((DEBUG_INFO, "SetVariable - %s\n", mIfrVariableName));
    Status = gRT->SetVariable(
      mIfrVariableName,
      &mMyWizardDriverFormSetGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof(MYWIZARDDRIVER_CONFIGURATION),
      Configuration   //  buffer is 000000  now
    );
    //
    // EFI variable for NV config doesn't exist, we should build this variable
    // based on default values stored in IFR
    //
    DEBUG((DEBUG_INFO, "HiiSetToDefaults - %s\n", ConfigRequestHdr));
    ActionFlag = HiiSetToDefaults(ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    ASSERT(ActionFlag);
  }
  else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    DEBUG((DEBUG_INFO, "HiiValidateSettings - %s\n", ConfigRequestHdr));
    ActionFlag = HiiValidateSettings(ConfigRequestHdr);
    ASSERT(ActionFlag);
  }
  FreePool(ConfigRequestHdr);

  DEBUG((DEBUG_INFO, "MyWizardDriverDriverEntryPoint: Exit, Status: 0x%x %r\n", Status, Status));
  return Status;
}