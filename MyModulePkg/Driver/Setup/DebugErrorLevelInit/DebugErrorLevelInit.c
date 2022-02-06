#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/HobLib.h>
#include <Setup/Setup.h>

EFI_STATUS
EFIAPI
DebugErrorLevelInitNotify (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mDebugErrorLevelInitNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiReadOnlyVariable2PpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) DebugErrorLevelInitNotify
};

EFI_STATUS
EFIAPI
DebugErrorLevelInitNotify (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableService;
  UINTN                           BufferSize;
  DEBUG_CONFIGURATION_MENU_VAR    Configuration;
  EFI_GUID                        VarGuid;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  Status = EFI_SUCCESS;
  VariableService = (EFI_PEI_READ_ONLY_VARIABLE2_PPI*) Ppi;
  BufferSize = sizeof (Configuration);
  VarGuid = (EFI_GUID) DEBUG_CONFIGURATION_MENU_FORMSET_GUID ;

  Status = VariableService->GetVariable (
                              VariableService,
                              DCM_IFR_VAR_NAME,
                              &VarGuid,
                              NULL,
                              &BufferSize,
                              &Configuration
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "GetVariable() failed to retrieve NV variable %s\n", DCM_IFR_VAR_NAME));
  }
  else {
    DEBUG ((DEBUG_INFO, "Configuration.SerialIoUartEnable: 0x%x\n", Configuration.SerialIoUartEnable));
    //
    // Build the GUID'ed HOB for DXE
    //
    BuildGuidDataHob(
      &gDebugConfigHobGuid,
      &Configuration,
      sizeof(Configuration)
      );
  }

  DEBUG ((DEBUG_INFO, "%a: Exit, Status: 0x%x - %r\n", __FUNCTION__, Status, Status));
  return Status;
}

EFI_STATUS
EFIAPI
DebugErrorLevelInitEntryPoint (
  IN EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  
  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  Status = PeiServicesNotifyPpi (&mDebugErrorLevelInitNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesNotifyPpi - 0x%x %r\n", Status, Status));
  }

  DEBUG ((DEBUG_INFO, "%a: Exit, Status: 0x%x - %r\n", __FUNCTION__, Status, Status));
  return Status;
}