Q1: How to compile this module?
A: 1. Goto edk2 workspace directory, where edk2, edk2-libc folders are present
   2. Open visual studio developer command prompt, and execute the following commands.
      set WORKSPACE=%CD%
	  set PACKAGES_PATH=%WORKSPACE%\edk2;%WORKSPACE%\edk2-libc
      set NASM_PREFIX=C:\nasm\
   3. cd edk2, and run edksetup.bat (if running for first time: edksetup.bat Rebuild)
   4. Add MyModulePkg/Application/MyHelloWorld/MyHelloWorld.inf in the Components section of MyModulePkg.dsc.
   5. build -p MyModulePkg\MyModulePkg.dsc -a X64 -t VS2017 -b DEBUG
   6. Check build output in \Build\MyModule\DEBUG_VS2017\X64\MyModulePkg\Application\MyHelloWorld\MyHelloWorld\DEBUG\.