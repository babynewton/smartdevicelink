%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AudioStreamingState_intHash
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
AUDIBLE,0
ATTENUATED,2
NOT_AUDIBLE,1
