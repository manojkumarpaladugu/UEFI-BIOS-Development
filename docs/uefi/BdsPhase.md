**Introduction:**  
The Boot Device Selection (BDS) Architectural Protocol executes during BDS phase. The BDS Architectural Protocol is discovered in DXE phase, and it is executed when two conditions are met.
1. All the DXE Architectural Protocols have been registered in the handle database. This condition is required for the DXE core to produce EFI Boot Services and EFI Runtime Services.
1. The DXE dispatcher doesn’t have no pending DXE drivers to be dispatched. This condition evaluates to true only when priori file in each firmware volume has been processed and the dependency expression is evaluated to true have been loaded and executed.

The BDS phase performs a well-defined set of tasks.
1. Initialize console devices base don ConIn, ConOut, and StdErr environment variables.
1. Attempt to load drivers listed in the Driver#### and DriverOrder environment variables.
1. Attempt to boot from the boot selections listed in the Boot#### and BootOrder environment variables.

If the BDS phase is failed to connect console devices, load a driver or boot a boot selection, it is required to reinvoke the DXE dispatcher.

**Console Devices:**  
1. VGA Adapters: These adapters can produce text-based display that is abstracted with Simple Text Output Protocol.
1. Video Adapters: These adapters can produce graphic based display that is abstracted with Graphics Output Protocol.
1. Serial Terminal: These can produce both Simple input and Simple Text Output Protocol.
1. Telnet:  These can produce both Simple input and Simple Text Output Protocol.
1. Remote Graphic Displays (HTTP): These can produce both Simple input and Simple Text Output Protocol.

**Boot Devices:**  
1. Block IO Protocol devices that are formatted with FAT file system.
1. Devices that directly produce File System Protocol.
1. Device that directly produce Load File Protocol.
1. Disk devices typically produces File System Protocol, and network devices typically produce Load File Protocol.

**Boot Services Terminate:**  
The BDS phase is terminated when OS loader is executed, and OS successfully booted. An OS loader or OS kernel may call single service ExitBootServices() to terminate BDS phase. Here after Runtime phase begins.
