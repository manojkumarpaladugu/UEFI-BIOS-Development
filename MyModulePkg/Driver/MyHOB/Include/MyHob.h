#ifndef __MY_HOB__
#define __MY_HOB__

typedef struct _STRUCT_HOB_ {
	EFI_HOB_GUID_TYPE EfiHobGuidType;
	UINT32 data;
}STRUCT_HOB;

// {7ED223EA-FF70-4029-AA0F-3876589A668B}
#define MY_HOB_GUID \
{ 0x7ed223ea, 0xff70, 0x4029, { 0xaa, 0xf, 0x38, 0x76, 0x58, 0x9a, 0x66, 0x8b } };

EFI_GUID gMyHobGuid = MY_HOB_GUID;

#endif