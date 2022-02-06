/** @file
  Debug Print Error Level library instance that retrieves the current error
  level from PcdDebugPrintErrorLevel.  This generic library instance does not
  support the setting of the global debug print error level mask for the platform.

  Copyright (c) 2011 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Setup/Setup.h>

/**
  Returns the debug print error level mask for the current module.

  @return  Debug print error level mask for the current module.

**/
UINT32
EFIAPI
GetDebugPrintErrorLevel (
  VOID
  )
{
  DEBUG_CONFIGURATION_MENU_VAR *Configuration;
  EFI_HOB_GUID_TYPE            *GuidHob;

  GuidHob = GetFirstGuidHob (&gDebugConfigHobGuid);
  if (GuidHob != NULL && GET_GUID_HOB_DATA_SIZE (GuidHob) != sizeof (Configuration)) {
    Configuration = (DEBUG_CONFIGURATION_MENU_VAR *) GET_GUID_HOB_DATA (GuidHob);
    if (Configuration != NULL)
      return Configuration->SerialIoUartEnable;
  }

  //
  // Retrieve the current debug print error level mask from PcdDebugPrintErrorLevel.
  //
  return PcdGet32(PcdDebugPrintErrorLevel);
}

/**
  Sets the global debug print error level mask fpr the entire platform.

  @param   ErrorLevel     Global debug print error level.

  @retval  TRUE           The debug print error level mask was sucessfully set.
  @retval  FALSE          The debug print error level mask could not be set.

**/
BOOLEAN
EFIAPI
SetDebugPrintErrorLevel (
  UINT32  ErrorLevel
  )
{
  //
  // This library uinstance does not support setting the global debug print error
  // level mask.
  //
  return FALSE;
}
