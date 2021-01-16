#include "MySmbiosDxe.h"

//
// Structure for My SMBIOS type 129 filled with default data
//
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE125 mSmbiosTableType125Data = {
  {
    SMBIOS_TYPE_125,               // SMBIOS type
    sizeof(SMBIOS_TABLE_TYPE125),  // Data length
    0x00                           // Dummy value for now, Smbios core driver will fill it later
  },
  { 0 },
  { 0 }
};

EFI_STATUS
CreateSmbiosTableType125 (
  IN SMBIOS_TABLE_TYPE125 *SmbiosTable,
  IN UINT8                SmbiosTableLength
)
{
  EFI_STATUS               Status = EFI_SUCCESS;
  EFI_SMBIOS_PROTOCOL      *Smbios;
  EFI_SMBIOS_TABLE_HEADER  *SmbiosRecord;
  EFI_SMBIOS_HANDLE        SmbiosHandle;

  DEBUG ((DEBUG_INFO, "CreateSmbiosTableType125: Entry\n"));

  if (SmbiosTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  //
  // Add two zeros at the end of structure. So that Smbios->Add will check the last two zeros as terminator.
  //
  SmbiosRecord = AllocateZeroPool (SmbiosTableLength + 2);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (SmbiosRecord, SmbiosTable, SmbiosTableLength);

  Status = Smbios->Add (Smbios, NULL, &SmbiosHandle, SmbiosRecord);

  FreePool (SmbiosRecord);

  DEBUG ((DEBUG_INFO, "CreateSmbiosTableType125: Exit\n"));
  return Status;
}

EFI_STATUS
GetSmbiosTable (
  IN SMBIOS_TYPE              SmbiosType,
  OUT EFI_SMBIOS_TABLE_HEADER **SmbiosTable
)
{
  EFI_STATUS              Status = EFI_SUCCESS;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_HANDLE       SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER *Record;

  if (SmbiosTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  do {
    Status = Smbios->GetNext(
                        Smbios,
                        &SmbiosHandle,
                        NULL,
                        &Record,
                        NULL
                      );
    if (EFI_ERROR(Status)) {
      return EFI_NOT_FOUND;
    }
    DEBUG ((DEBUG_INFO, "SMBIOS Table - address: 0x%X, type: 0x%X\n", Record, Record->Type));
  } while (Record->Type != SmbiosType);

  *SmbiosTable = Record;

  return Status;
}

EFI_STATUS
UpdateSmbiosTableType125 (
  VOID
)
{
  EFI_STATUS               Status = EFI_SUCCESS;
  EFI_SMBIOS_TABLE_HEADER  *Record;
  SMBIOS_TABLE_TYPE125     *SmbiosTableType125Data;

  DEBUG((DEBUG_INFO, "UpdateSmbiosTableType125: Entry\n"));

  Status = GetSmbiosTable (SMBIOS_TYPE_125, &Record);
  if (EFI_ERROR(Status)) {
    return EFI_NOT_FOUND;
  }

  SmbiosTableType125Data = (SMBIOS_TABLE_TYPE125 *) Record;

  SmbiosTableType125Data->Struct1.XSupport = TRUE;
  SmbiosTableType125Data->Struct1.YSupport = FALSE;
  SmbiosTableType125Data->Struct1.ZSupport = TRUE;

  SmbiosTableType125Data->Struct2.Bus = 1;
  SmbiosTableType125Data->Struct2.Device = 31;
  SmbiosTableType125Data->Struct2.Function = 3;
  SmbiosTableType125Data->Struct2.DeviceId = 0x1234;

  DEBUG ((DEBUG_INFO, "Updated SmbiosTableType125Data (0x%X) with below data:\n", SmbiosTableType125Data));
  for (UINT8 i = 0; i < sizeof(SMBIOS_TABLE_TYPE125) + 2; i++) {
    DEBUG ((DEBUG_INFO, "%X ", ((UINT8*)SmbiosTableType125Data)[i]));
  }
  DEBUG((DEBUG_INFO, "\n"));

  DEBUG((DEBUG_INFO, "UpdateSmbiosTableType125: Exit\n"));
  return Status;
}

EFI_STATUS
EFIAPI
MySmbiosDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  DEBUG((DEBUG_INFO, "MySmbiosDxeEntryPoint: Entry\n"));

  Status = CreateSmbiosTableType125 (&mSmbiosTableType125Data, sizeof(mSmbiosTableType125Data));
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  Status = UpdateSmbiosTableType125 ();
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  DEBUG((DEBUG_INFO, "MySmbiosDxeEntryPoint: Exit\n"));
  return Status;
}