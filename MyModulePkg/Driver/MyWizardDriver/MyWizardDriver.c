/** @file
  TODO: Brief Description of UEFI Driver MyWizardDriver
  
  TODO: Detailed Description of UEFI Driver MyWizardDriver

  TODO: Copyright for UEFI Driver MyWizardDriver
  
  TODO: License for UEFI Driver MyWizardDriver

**/

#include "MyWizardDriver.h"
// Begin code 
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
// end code

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
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;
  UINTN       HandleCount;
  UINTN       Index;

  Status = EFI_SUCCESS;
  //
  // Retrieve array of all handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (
                  AllHandles,
                  NULL,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Disconnect the current driver from handles in the handle database 
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->DisconnectController (HandleBuffer[Index], gImageHandle, NULL);
  }

  //
  // Free the array of handles
  //
  FreePool (HandleBuffer);

  //
  // Do any additional cleanup that is required for this driver
  //
  if (DummyBufferfromStart != NULL) {
	  FreePool(DummyBufferfromStart);
	  DEBUG((EFI_D_INFO, "[MyWizardDriver] Unload, clear buffer\r\n"));
  }
  DEBUG((EFI_D_INFO, "[MyWizardDriver] Unload success\r\n"));

  return EFI_SUCCESS;
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
  EFI_HII_PACKAGE_LIST_HEADER *PackageListHeader;
  EFI_HII_DATABASE_PROTOCOL   *HiiDatabase;
  EFI_HII_HANDLE              HiiHandle[2];
  EFI_STRING                  ConfigRequestHdr;
  UINTN                       BufferSize;

  Status = EFI_SUCCESS;

  //Now do HII Stuff
// Initialize the local variables.
  ConfigRequestHdr = NULL;

  // Initialize driver private data
  PrivateData = AllocateZeroPool(sizeof(MYWIZARDDRIVER_DEV));
  if (PrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PrivateData->Signature = MYWIZARDDRIVER_DEV_SIGNATURE;

  PrivateData->ConfigAccess.ExtractConfig = MyWizardDriverHiiConfigAccessExtractConfig;
  PrivateData->ConfigAccess.RouteConfig = MyWizardDriverHiiConfigAccessRouteConfig;
  PrivateData->ConfigAccess.Callback = MyWizardDriverHiiConfigAccessCallback;

  //
  // Publish sample Fromset and config access 
  //
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
  // end code

  //
  // Retrieve HII Package List Header on ImageHandle
  //
  Status = gBS->OpenProtocol (
                  ImageHandle,
                  &gEfiHiiPackageListProtocolGuid,
                  (VOID **)&PackageListHeader,
                  ImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Retrieve the pointer to the UEFI HII Database Protocol 
    //
    Status = gBS->LocateProtocol (
                    &gEfiHiiDatabaseProtocolGuid, 
                    NULL, 
                    (VOID **)&HiiDatabase
                    );
    if (!EFI_ERROR (Status)) {
      //
      // Register list of HII packages in the HII Database
      //
      Status = HiiDatabase->NewPackageList (
                              HiiDatabase, 
                              PackageListHeader,
                              mDriverHandle[0],
                              &HiiHandle[0]
                              );
      ASSERT_EFI_ERROR (Status);
    }
  }
  Status = EFI_SUCCESS;

  // Begin code
  PrivateData->HiiHandle[0] = HiiHandle[0];

  BufferSize = sizeof(MYWIZARDDRIVER_CONFIGURATION);

  // Create NV Variable
  // IF driver is not part of the Platform then need to get/set defaults for the NVRAM configuration that the driver will use.
  Status = gRT->GetVariable(
    mIfrVariableName,
    &mMyWizardDriverFormSetGuid,
    NULL,
    &BufferSize,
    &PrivateData->Configuration
  );
  if (EFI_ERROR(Status)) {  // Not definded yet so add it to the NV Variables.
        // zero out buffer
    ZeroMem(&PrivateData->Configuration, sizeof(MYWIZARDDRIVER_CONFIGURATION));
    Status = gRT->SetVariable(
      mIfrVariableName,
      &mMyWizardDriverFormSetGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof(MYWIZARDDRIVER_CONFIGURATION),
      &PrivateData->Configuration   //  buffer is 000000  now
    );
  }
  // end code

  DEBUG((DEBUG_INFO, "MyWizardDriverDriverEntryPoint: Exit, Status: 0x%x %r\n", Status, Status));
  return Status;
}