[Back to Index](../index.md)

```diff
+ **EFI SCSI Driver Stack Overview:**  
The EFI SCSI Driver Stack includes the SCSI Pass Thru Driver, SCSI Bus Driver and individual SCSI Device Drivers.

**SCSI Host Controller Driver:** It manages a SCSI Host Controller that contains one or more SCSI buses. It creates SCSI Bus Controller Handles on each SCSI bus, and consumes PCI IO Protocol and attaches Extended SCSI Pass Thru Protocol and Device Path Protocol on each handle the driver produced.

**SCSI Bus Driver:** A SCSI Bus Driver manages a SCSI Bus Controller Handle that is created by SCSI Host Controller Driver. It creates SCSI Device Handles for each SCSI Device Controller detected during SCSI bus enumeration, and consumes SCSI Pass Thru Protocol and installs SCSI IO Protocol and Device Path Protocol on each handle the driver produced.

**SCSI Device Driver:** A SCSI Device Driver manages one kind of SCSI Device. Device handles for SCSI
Devices are created by SCSI Bus Drivers. A SCSI Device Driver could be a bus driver itself, and may create child handles. But most SCSI Device Drivers will be device drivers that do not create new handles. For the pure device driver, it attaches protocol instance to the device handle of the SCSI Device. Consumes SCSI IO Protocol and produces I/O abstractions that allow the SCSI Device to be used in the pre-boot environment. The most common I/O abstractions are used to boot an EFI compliant OS.

Reference: UEFI Spec 2.8 May 2020.pdf

[Back to Index](../index.md)
