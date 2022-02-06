#include <Uefi.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include "DebugConfigurationMenu.h"

/**

  This function allows the caller to request the current
  configuration for one or more named elements. The resulting
  string is in <ConfigAltResp> format. Any and all alternative
  configuration strings shall also be appended to the end of the
  current configuration string. If they are, they must appear
  after the current configuration. They must contain the same
  routing (GUID, NAME, PATH) as the current configuration string.
  They must have an additional description indicating the type of
  alternative configuration the string represents,
  "ALTCFG=<StringToken>". That <StringToken> (when
  converted from Hex UNICODE to binary) is a reference to a
  string in the associated string pack.

  @param This       Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param Request    A null-terminated Unicode string in
                    <ConfigRequest> format. Note that this
                    includes the routing information as well as
                    the configurable name / value pairs. It is
                    invalid for this string to be in
                    <MultiConfigRequest> format.
                    If a NULL is passed in for the Request field,
                    all of the settings being abstracted by this function
                    will be returned in the Results field.  In addition,
                    if a ConfigHdr is passed in with no request elements,
                    all of the settings being abstracted for that particular
                    ConfigHdr reference will be returned in the Results Field.

  @param Progress   On return, points to a character in the
                    Request string. Points to the string's null
                    terminator if request was successful. Points
                    to the most recent "&" before the first
                    failing name / value pair (or the beginning
                    of the string if the failure is in the first
                    name / value pair) if the request was not
                    successful.

  @param Results    A null-terminated Unicode string in
                    <MultiConfigAltResp> format which has all values
                    filled in for the names in the Request string.
                    String to be allocated by the called function.

  @retval EFI_SUCCESS             The Results string is filled with the
                                  values corresponding to all requested
                                  names.

  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.

  @retval EFI_NOT_FOUND           A configuration element matching
                                  the routing data is not found.
                                  Progress set to the first character
                                  in the routing header.

  @retval EFI_INVALID_PARAMETER   Illegal syntax. Progress set
                                  to most recent "&" before the
                                  error or the beginning of the
                                  string.

  @retval EFI_INVALID_PARAMETER   Unknown name. Progress points
                                  to the & before the name in
                                  question.

**/
EFI_STATUS
DcmExtractConfig (
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN CONST  EFI_STRING                     Request,
  OUT       EFI_STRING                     *Progress,
  OUT       EFI_STRING                     *Results
  )
{
  EFI_STATUS                      Status;
  DCM_PRIVATE_DATA                *DcmPrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL *HiiConfigRoutingProtocol;
  UINTN                           BufferSize;
  EFI_STRING                      ConfigRequest;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status        = EFI_SUCCESS;
  ConfigRequest = NULL;
  *Progress = Request;
  DcmPrivateData = DCM_PRIVATE_FROM_THIS (This);
  HiiConfigRoutingProtocol = DcmPrivateData->HiiConfigRoutingProtocol;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (DEBUG_CONFIGURATION_MENU_VAR);
  Status = gRT->GetVariable (
                  DcmPrivateData->IfrVariableName,
                  &DcmPrivateData->FormSetGuid,
                  NULL,
                  &BufferSize,
                  &DcmPrivateData->Configuration
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a: Request=%s\n", __FUNCTION__, Request));

  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  if (!HiiIsConfigHdrMatch(Request, &DcmPrivateData->FormSetGuid, NULL)) {
    return EFI_NOT_FOUND;
  }

  Status = HiiConfigRoutingProtocol->BlockToConfig (
                                      HiiConfigRoutingProtocol,
                                      Request,
                                      (UINT8*)&DcmPrivateData->Configuration,
                                      BufferSize,
                                      Results,
                                      Progress
                                      );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: BlockToConfig(): %r, Progress=%s\n",
      __FUNCTION__, Status, (Status == EFI_DEVICE_ERROR) ? NULL : *Progress));
  }
  else {
    DEBUG ((DEBUG_INFO, "%a: Results=%s\n", __FUNCTION__, *Results));
  }

  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  }
  else if (StrStr(Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen(Request);
  }

  DEBUG ((DEBUG_INFO, "%a: Exit, Status: 0x%x - %r\n", __FUNCTION__, Status, Status));
  return Status;
}

/**

  This function applies changes in a driver's configuration.
  Input is a Configuration, which has the routing data for this
  driver followed by name / value configuration pairs. The driver
  must apply those pairs to its configurable storage. If the
  driver's configuration is stored in a linear block of data
  and the driver's name / value pairs are in <BlockConfig>
  format, it may use the ConfigToBlock helper function (above) to
  simplify the job.

  @param This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param Configuration  A null-terminated Unicode string in
                        <ConfigString> format.

  @param Progress       A pointer to a string filled in with the
                        offset of the most recent '&' before the
                        first failing name / value pair (or the
                        beginn ing of the string if the failure
                        is in the first name / value pair) or
                        the terminating NULL if all was
                        successful.

  @retval EFI_SUCCESS             The results have been distributed or are
                                  awaiting distribution.

  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.

  @retval EFI_INVALID_PARAMETERS  Passing in a NULL for the
                                  Results parameter would result
                                  in this type of error.

  @retval EFI_NOT_FOUND           Target for the specified routing data
                                  was not found

**/
EFI_STATUS
DcmRouteConfig (
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN CONST  EFI_STRING                     Configuration,
  OUT       EFI_STRING                     *Progress
  )
{
  EFI_STATUS                      Status;
  DCM_PRIVATE_DATA                *DcmPrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL *HiiConfigRoutingProtocol;
  UINTN                           BufferSize;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;

  DcmPrivateData = DCM_PRIVATE_FROM_THIS (This);
  HiiConfigRoutingProtocol = DcmPrivateData->HiiConfigRoutingProtocol;
  *Progress = Configuration;

  DEBUG ((DEBUG_INFO, "Configuration: %s\n", Configuration));

  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  if (!HiiIsConfigHdrMatch(Configuration, &DcmPrivateData->FormSetGuid, NULL)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (DEBUG_CONFIGURATION_MENU_VAR);
  Status = gRT->GetVariable (
                  DcmPrivateData->IfrVariableName,
                  &DcmPrivateData->FormSetGuid,
                  NULL,
                  &BufferSize,
                  &DcmPrivateData->Configuration
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  Status = HiiConfigRoutingProtocol->ConfigToBlock (
                                      HiiConfigRoutingProtocol,
                                      Configuration,
                                      (UINT8*)&DcmPrivateData->Configuration,
                                      &BufferSize,
                                      Progress
                                      );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Store Buffer Storage back to EFI variable
  //
  Status = gRT->SetVariable (
                  DcmPrivateData->IfrVariableName,
                  &DcmPrivateData->FormSetGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (DEBUG_CONFIGURATION_MENU_VAR),
                  &DcmPrivateData->Configuration
                  );

  DEBUG ((DEBUG_INFO, "%a: Exit, Status: 0x%x - %r\n", __FUNCTION__, Status, Status));
  return Status;
}

/**

  This function is called to provide results data to the driver.
  This data consists of a unique key that is used to identify
  which data is either being passed back or being asked for.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect. The format of the data tends to
                                 vary based on the opcode that generated the callback.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
EFI_STATUS
DcmCallback (
  IN     CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN     EFI_BROWSER_ACTION                   Action,
  IN     EFI_QUESTION_ID                      QuestionId,
  IN     UINT8                                Type,
  IN OUT EFI_IFR_TYPE_VALUE                   *Value,
  OUT    EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_CONFIG_ACCESS_PROTOCOL gDcmHiiConfigAccessProtocol = {
  DcmExtractConfig,
  DcmRouteConfig,
  DcmCallback
};