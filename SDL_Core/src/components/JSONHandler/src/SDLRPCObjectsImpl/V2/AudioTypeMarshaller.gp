%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AudioType_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPCV2::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
PCM,0
