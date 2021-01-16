#
# DXE Drivers
#
  MyModulePkg/Driver/MyDXE/MyDXE.inf
  MyModulePkg/Driver/MyHOB/MyHobDxe/MyHobDxe.inf
  MyModulePkg/Driver/MyDXEEventNotify/MyDXEEventNotify.inf
  MyModulePkg/Driver/MyProtocol/MyProtocolDXE1/MyProtocolDXE1.inf
  MyModulePkg/Driver/MyProtocol/MyProtocolDXE2/MyProtocolDXE2.inf
  MyModulePkg/Driver/MyDXESMM/MyDXESMM.inf
  MyModulePkg/Driver/MySmbiosDxe/MySmbiosDxe.inf

#
# UEFI Drivers
#
  MyModulePkg/Driver/MyServiceDriver/MyServiceDriver.inf
  MyModulePkg/Driver/EvtExitBootServices/EvtExitBootServices.inf
  MyModulePkg/Driver/GetUefiVariableStorageInfo/GetUefiVariableStorageInfo.inf
  MyModulePkg/Driver/MyWizardDriver/MyWizardDriver.inf
  MyModulePkg/Driver/OneShotTimerEvent/OneShotTimerEvent.inf
  MyModulePkg/Driver/PeriodicTimerEvent/PeriodicTimerEvent.inf
  MyModulePkg/Driver/SerialIoUefiDriver/SerialIoUefiDriver.inf
  MyModulePkg/Driver/WaitKeyPressOrTimerEvent/WaitKeyPressOrTimerEvent.inf
  MyModulePkg/Driver/WriteReadUefiVariable/WriteReadUefiVariable.inf

#
# UEFI Applications
#
  MyModulePkg/Application/MyHelloWorld/MyHelloWorld.inf
  MyModulePkg/Application/MyDXESMMApp/MyDXESMMApp.inf
  MyModulePkg/Application/MyPCIeEnumeration/MyPCIeEnumeration.inf
  MyModulePkg/Application/MySataBlockIoReader/MySataBlockIoReader.inf
  MyModulePkg/Application/MyAcpiFadt/MyAcpiFadt.inf
  MyModulePkg/Application/MyS3BootScriptTable/MyS3BootScriptTable.inf
  MyModulePkg/Application/MyServiceDriverApp/MyServiceDriverApp.inf
  MyModulePkg/Application/CountPCIControllers/CountPCIControllers.inf
  MyModulePkg/Application/ExampleEvent/ExampleEvent.inf