#ifndef _SETUP_H_
#define _SETUP_H_

#include <Guid/HiiPlatformSetupFormset.h>

// {DEB58765-ABEA-4BE6-B411-E226A1256F61}
#define DEBUG_CONFIGURATION_MENU_FORMSET_GUID \
{ 0xdeb58765, 0xabea, 0x4be6, { 0xb4, 0x11, 0xe2, 0x26, 0xa1, 0x25, 0x6f, 0x61 } }

#define DEBUG_CONFIGURATION_MENU_VARID 0x1122

#define DEBUG_CONFIGURATION_MENU_FORM1_ID 0x1001

#define SERIALIO_UART_QUESTIONID 0x2001

#pragma pack(push, 1)
typedef struct _DEBUG_CONFIGURATION_MENU_VAR {
  BOOLEAN SerialIoUartEnable;
}DEBUG_CONFIGURATION_MENU_VAR;
#pragma pack(pop)

//
// To dump this NV variable in EFI shell: dmpstore -all DcmIfrNvData
//
#define DCM_IFR_VAR_NAME L"DcmIfrNvData"
#endif // _SETUP_H_