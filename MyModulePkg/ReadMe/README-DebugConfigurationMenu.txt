1) Add DebugConfigurationMenu.inf to along with DXE modules of OvmfPkgIa32.dsc
!if $(SMM_REQUIRE) == TRUE
  MyModulePkg/Driver/Setup/DebugConfigurationMenu/DebugConfigurationMenu.inf
!endif

2) Add DebugConfigurationMenu.inf to DxeModules.fdf -> [FV.DXEFV]
!if $(SMM_REQUIRE) == TRUE
INF MyModulePkg/Driver/Setup/DebugConfigurationMenu/DebugConfigurationMenu.inf
!endif

3) Add DebugErrorLevelInit.inf to along with PEI modules of OvmfPkgIa32.dsc
!if $(SMM_REQUIRE) == TRUE
  MyModulePkg/Driver/Setup/DebugErrorLevelInit/DebugErrorLevelInit.inf
!endif

4) Add DebugErrorLevelInit.inf to PeiModules.fdf -> [FV.PEIFV]
!if $(SMM_REQUIRE) == TRUE
INF MyModulePkg/Driver/Setup/DebugErrorLevelInit/DebugErrorLevelInit.inf
!endif

5) Add this DebugPrintErrorLevelLib in OvmfPkgIa32.dsc -> [LibraryClasses.common.PEIM], [LibraryClasses.common.DXE_CORE],
   [LibraryClasses.common.DXE_RUNTIME_DRIVER], [LibraryClasses.common.UEFI_DRIVER], [LibraryClasses.common.DXE_DRIVER]
!if $(SMM_REQUIRE) == TRUE
  DebugPrintErrorLevelLib|MyModulePkg/Library/PeiDxeSmmDebugPrintErrorLevelLib/PeiDxeSmmDebugPrintErrorLevelLib.inf
!endif

6) Build command
     build -p OvmfPkg\OvmfPkgIa32.dsc -a IA32 -b DEBUG -t VS2019 -D SMM_REQUIRE

7) Qemu command
     "C:\Program Files\qemu\qemu-system-x86_64.exe" -machine q35,smm=on,accel=tcg -global driver=cfi.pflash01,property=secure,value=on -drive if=pflash,format=raw,unit=0,file=%WORKSPACE%\Build\OvmfIa32\DEBUG_VS2019\FV\OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=%WORKSPACE%\Build\OvmfIa32\DEBUG_VS2019\FV\OVMF_VARS.fd -debugcon file:%WORKSPACE%\OvmfPkgIa32_UART.log -global isa-debugcon.iobase=0x402 -net none
