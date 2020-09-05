Q1: How to compile this module?
A: 1. Goto edk2 workspace directory, where edk2, edk2-libc folders are present
   2. Open visual studio developer command prompt, and execute the following commands.
      set WORKSPACE=%CD%
	  set PACKAGES_PATH=%WORKSPACE%\edk2;%WORKSPACE%\edk2-libc
      set NASM_PREFIX=C:\nasm\
   3. cd edk2, and run edksetup.bat (if running for first time: edksetup.bat Rebuild)
   4. Add MyModulePkg/Driver/MyPEIM/MyPEIM.inf in the Components section of MyModulePkg.dsc.
   5. build -p MyModulePkg\MyModulePkg.dsc -a X64 -t VS2017 -b DEBUG
   6. Check build output in \Build\MyModule\DEBUG_VS2017\X64\MyModulePkg\Driver\MyPEIM\MyPEIM\DEBUG\.


Q2: How to see the output of this driver in OVMF?
A: 1. Goto edk2 workspace directory, where edk2, edk2-libc folders are present
   2. Open visual studio developer command prompt, and execute the following commands.
      set WORKSPACE=%CD%
	  set PACKAGES_PATH=%WORKSPACE%\edk2;%WORKSPACE%\edk2-libc
      set NASM_PREFIX=C:\nasm\
   3. cd edk2, and run edksetup.bat (if running for first time: edksetup.bat Rebuild)
   4. Add MyModulePkg/Driver/MyPEIM/MyPEIM.inf in the PEI OvmfPkgX64.fdf and OvmfPkgX64.dsc of PEI modules section.
   5. build -p OvmfPkg\OvmfPkgX64.dsc -D SOURCE_DEBUG_ENABLE=TRUE -a X64 -t VS2017 -b DEBUG
   6. Open command prompt as admin, and execute cd C:\Program Files\qemu
   7. qemu-system-x86_64 -debugcon file:debug.log -global isa-debugcon.iobase=0x402 -net none -bios \Build\OvmfX64\DEBUG_VS2017\FV\OVMF.fd
   8. Open debug.log and check output.