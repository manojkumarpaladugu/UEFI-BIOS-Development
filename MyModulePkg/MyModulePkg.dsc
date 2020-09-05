## @file
# EFI/PI Reference Module Package for All Architectures
#
##

[Defines]
  PLATFORM_NAME                  = MyModule
  PLATFORM_GUID                  = AA4737C1-8604-4B08-A344-39A0E3F1FC93
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/MyModule
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT

[LibraryClasses]
  #
  # Entry point
  #
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  #
  # Generic Modules
  #
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  #
  # Misc
  #
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf

[LibraryClasses.common.DXE_DRIVER]
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf

[LibraryClasses.common.UEFI_APPLICATION]
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf

[Components]
#
# PEI Drivers
#
#  MyModulePkg/Driver/MyPEIM/MyPEIM.inf
#  MyModulePkg/Driver/MyPPI/MyPEIM1/MyPEIM1.inf
#  MyModulePkg/Driver/MyPPI/MyPEIM2/MyPEIM2.inf
#  MyModulePkg/Driver/MyHOB/MyHobPeim/MyHobPeim.inf

#
# DXE Drivers
#
#  MyModulePkg/Driver/MyDXE/MyDXE.inf
#  MyModulePkg/Driver/MyHOB/MyHobDxe/MyHobDxe.inf
#  MyModulePkg/Driver/MyDXEEventNotify/MyDXEEventNotify.inf
#  MyModulePkg/Driver/MyProtocol/MyProtocolDXE1/MyProtocolDXE1.inf
#  MyModulePkg/Driver/MyProtocol/MyProtocolDXE2/MyProtocolDXE2.inf
#  MyModulePkg/Driver/MyDXESMM/MyDXESMM.inf
#  MyModulePkg/Driver/SerialIoUefiDriver/SerialIoUefiDriver.inf

#
#  UEFI Drivers
#
#  MyModulePkg/Driver/MyServiceDriver/MyServiceDriver.inf

#
# UEFI Applications
#
#  MyModulePkg/Application/MyHelloWorld/MyHelloWorld.inf
#  MyModulePkg/Application/MyDXESMMApp/MyDXESMMApp.inf
#  MyModulePkg/Application/MyPCIeEnumeration/MyPCIeEnumeration.inf
#  MyModulePkg/Application/MySataBlockIoReader/MySataBlockIoReader.inf
#  MyModulePkg/Application/MyAcpiFadt/MyAcpiFadt.inf
#  MyModulePkg/Application/MyS3BootScriptTable/MyS3BootScriptTable.inf
#  MyModulePkg/Application/MyServiceDriverApp/MyServiceDriverApp.inf
  MyModulePkg/Application/CountPCIControllers/CountPCIControllers.inf
  MyModulePkg/Application/ExampleEvent/ExampleEvent.inf