#define EXAMPLE_CONFIGURATION_VARIABLE_NAME L"ExampleConfiguration"

//
// Example configuration structure from ExampleConfigurationVariable.h
//
typedef struct {
  UINT32 Question1;
  UINT16 Question2;
  UINT8 Question3;
}EXAMPLE_CONFIGURATION;

extern EFI_GUID gExampleConfigurationVariableGuid;