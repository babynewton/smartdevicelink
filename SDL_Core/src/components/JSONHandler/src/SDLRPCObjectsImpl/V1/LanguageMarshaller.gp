%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Language_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPC::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
EN-US,0
ES-MX,1
FR-CA,2
