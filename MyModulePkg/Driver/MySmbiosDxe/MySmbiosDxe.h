#ifndef _MY_SMBIOS_DXE_H_
#define _MY_SMBIOS_DXE_H_

#include <Uefi.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

#define SMBIOS_TYPE_125  125

typedef struct _STRUCT1 {
  UINT32 XSupport : 1;  // [0]
  UINT32 YSupport : 1;  // [1]
  UINT32 ZSupport : 1;  // [2] 
  UINT32 Reserved : 29; // [31:3]
}STRUCT1;

typedef struct _STRUCT2 {
  UINT32 Function : 3;  // [2:0]
  UINT32 Device   : 5;  // [7:3]
  UINT32 Bus      : 8;  // [15:8]
  UINT32 DeviceId : 16; // [31:16]
  UINT32 Reserved : 32; // [63:32]
}STRUCT2;

typedef struct _SMBIOS_TABLE_TYPE125 {
  SMBIOS_STRUCTURE Header;
  STRUCT1          Struct1;
  STRUCT2          Struct2;
}SMBIOS_TABLE_TYPE125;

#endif // _MY_SMBIOS_DXE_H_