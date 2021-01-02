
[Back to Index](../index.md)

**EDK2 Development Setup**  
1. Install Visual Studio 2019  
    - Run Visual Studio installer  
    - Select  
       Workloads->Desktop development with C++  
       Individual components->MSVC v142 - VS 2019 C++ x64/x86 Spectre-mitigated libs (latest)  
    - If EDK source code requires old toolset for compilation, then install below component from VS 2019 installer. Otherwise skip this step.  
       Individual components->MSVC v140 - VS 2015 C++ build tools(v14.0)
        1. Also, install Windows SDK 8.1
  
2. Download ASL compiler to C:\ASL and NASM assembler to C:\NASM.  
    Refer https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
  
3. Install Python 3.x  
  
4. Clone EDK2 source code from latest stable branch.  
    git clone -b stable/202011 https://github.com/tianocore/edk2.git  
    cd edk2  
    git submodule update --init  
  
5. Build source code  
    set WORKSPACE=%cd%  
    set PACKAGES_PATH=%WORKSPACE%\edk2  
    set NASM_PREFIX=C:\NASM  
    cd edk2  
    edksetup.bat Rebuild  
    build -p OvmfPkg/OvmfPkgx64.dsc -D DEBUG_ON_SERIAL_PORT -a X64 -b DEBUG -t VS2019  
  
6. Run OVMF on QEMU  
        - To run OVMF with serial log  
		"C:\Program Files\qemu\qemu-system-x86_64.exe" -drive file=fat:rw:%WORKSPACE%\temp\,format=raw -debugcon file:%WORKSPACE%\OvmfPkg_X64_UART.log -global isa-debugcon.iobase=0x402 -net none -bios %WORKSPACE%\Build\OvmfX64\DEBUG_VS2019\FV\OVMF.fd  
	- To run OVMF without serial log  
		"C:\Program Files\qemu\qemu-system-x86_64.exe" -drive file=fat:rw:%WORKSPACE%\temp\,format=raw -net none -bios %WORKSPACE%\Build\OvmfX64\DEBUG_VS2019\FV\OVMF.fd

	- To run OVMF without virtual drive and serial log  
		"C:\Program Files\qemu\qemu-system-x86_64.exe" -net none -bios %WORKSPACE%\Build\OvmfX64\DEBUG_VS2019\FV\OVMF.fd

[Back to Index](../index.md)
