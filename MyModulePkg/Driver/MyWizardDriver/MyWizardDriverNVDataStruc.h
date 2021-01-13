
#ifndef _MYWIZARDDRIVERNVDATASTRUC_H_
#define _MYWIZARDDRIVERNVDATASTRUC_H_
#include <Guid/HiiPlatformSetupFormset.h>
#include <Guid/HiiFormMapMethodGuid.h>

#define MYWIZARDDRIVER_VAR_GUID   { 0x363729f9, 0x35fc, 0x40a6,{ 0xaf, 0xc8, 0xe8, 0xf5, 0x49, 0x11, 0xf1, 0xd6} } 

// {E16C7486-11B5-4180-9614-3C3BCCA71A6B}
#define MYWIZARDDRIVER_FORMSET_GUID \
{ 0xe16c7486, 0x11b5, 0x4180, { 0x96, 0x14, 0x3c, 0x3b, 0xcc, 0xa7, 0x1a, 0x6b } }

#define CONFIGURATION_VARSTORE_ID    0x1234

#define CONFIG_NAME_MIN_LENGTH 3
#define CONFIG_NAME_MAX_LENGTH 20

#pragma pack(1)
typedef struct {
  BOOLEAN DeviceEnable;
  CHAR16  ConfigName[CONFIG_NAME_MAX_LENGTH];
  UINT8   DeviceSpeed;
  UINT16  MaxStorageSizePerVolume;
} MYWIZARDDRIVER_CONFIGURATION;
#pragma pack()

#define MWD_MAIN_FORM_ID              0x1001

#define CONFIG_NAME_QUESTION_ID       0x2001
#define DEVICE_SPEED_QUESTION_ID      0x2002
#define MAX_SIZE_CHANNEL_QUESTION_ID  0x2003
#define DEVICE_ENABLE_QUESTION_ID     0x2004

#define DEVICE_SPEED_SLOW_MODE        0x01
#define DEVICE_SPEED_FAST_MODE        0x02
#define DEVICE_SPEED_ULTRA_FAST_MODE  0x03

#endif
