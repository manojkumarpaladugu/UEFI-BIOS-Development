#include "DebugConfigurationMenu.h"

DCM_PRIVATE_DATA *DcmPrivateData = NULL;

EFI_STATUS
EFIAPI
DebugConfigurationMenuEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      DriverHandle;
  EFI_HII_HANDLE                  HiiHandle;
  EFI_STRING                      ConfigRequestHdr;
  UINTN                           BufferSize;
  BOOLEAN                         ActionFlag;
  EFI_HII_CONFIG_ROUTING_PROTOCOL *HiiConfigRoutingProtocol;

  Status       = EFI_SUCCESS;
  DriverHandle = NULL;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  DcmPrivateData = AllocateZeroPool (sizeof(DCM_PRIVATE_DATA));
  if (DcmPrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DcmPrivateData->Signature = DCM_SIGNATURE;
  DcmPrivateData->HiiConfigAccessProtocol = gDcmHiiConfigAccessProtocol;
  DcmPrivateData->IfrVariableName = AllocateZeroPool (sizeof (DCM_IFR_VAR_NAME));
  if (DcmPrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (DcmPrivateData->IfrVariableName, DCM_IFR_VAR_NAME, sizeof (DCM_IFR_VAR_NAME));
  DcmPrivateData->FormSetGuid = (EFI_GUID)DEBUG_CONFIGURATION_MENU_FORMSET_GUID;

  //
  // Locate ConfigRouting protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID **)&HiiConfigRoutingProtocol);
  ASSERT_EFI_ERROR(Status);
  DcmPrivateData->HiiConfigRoutingProtocol = HiiConfigRoutingProtocol;

  //
  // Publish fromset and config access 
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mDcmHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &DcmPrivateData->HiiConfigAccessProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);
  DcmPrivateData->DriverHandle = DriverHandle;

  HiiHandle = HiiAddPackages (
                &DcmPrivateData->FormSetGuid,
                DcmPrivateData->DriverHandle,
                DebugConfigurationMenuStrings,
                DebugConfigurationMenuFormsBin,
                NULL
                );

  //
  // Initialize configuration data
  //
  ZeroMem (&DcmPrivateData->Configuration, sizeof(DEBUG_CONFIGURATION_MENU_VAR));

  ConfigRequestHdr = HiiConstructConfigHdr (&DcmPrivateData->FormSetGuid, DcmPrivateData->IfrVariableName, DcmPrivateData->DriverHandle);
  ASSERT(ConfigRequestHdr != NULL);

  //
  // Try to read NV config EFI variable first
  //
  BufferSize = sizeof (DEBUG_CONFIGURATION_MENU_VAR);
  Status = gRT->GetVariable (
                  DcmPrivateData->IfrVariableName,
                  &DcmPrivateData->FormSetGuid,
                  NULL,
                  &BufferSize,
                  &DcmPrivateData->Configuration
                  );
  if (EFI_ERROR(Status)) {  // Not definded yet so add it to the NV Variables.
    // zero out buffer
    ZeroMem (&DcmPrivateData->Configuration, sizeof(DEBUG_CONFIGURATION_MENU_VAR));
    Status = gRT->SetVariable (
                    DcmPrivateData->IfrVariableName,
                    &DcmPrivateData->FormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(DEBUG_CONFIGURATION_MENU_VAR),
                    &DcmPrivateData->Configuration   //  buffer is 000000  now
                    );

    //
    // EFI variable for NV config doesn't exist, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    ASSERT(ActionFlag);
  }
  else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    ASSERT(ActionFlag);
  }
  FreePool (ConfigRequestHdr);

  DEBUG((DEBUG_INFO, "%a: Exit, Status: 0x%x (%r)\n", __FUNCTION__, Status, Status));
  return Status;
}