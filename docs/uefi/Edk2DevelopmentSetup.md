
**EDK2 Development Setup**
1) Install Visual Studio 2019
    1) Run Visual Studio installer
    1) Select 
       Workloads->Desktop development with C++
       Individual components->MSVC v142 - VS 2019 C++ x64/x86 Spectre-mitigated libs (latest)
    1) If EDK source code requires old toolset for compilation, then install below component from VS 2019 installer. Otherwise skip this step
        Individual components->MSVC v140 - VS 2015 C++ build tools(v14.0)
        1) Also, install Windows SDK 8.1

1) Download ASL compiler to C:\ASL and NASM assembler to C:\NASM.
    Refer https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
	 
1) Install Python 3.x

1) Clone EDK2 source code from latest stable branch.
    git clone -b stable/202011 https://github.com/tianocore/edk2.git
    cd edk2
    git submodule update --init
