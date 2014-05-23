%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name VehicleDataType_intHash
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
VEHICLEDATA_GPS,0
VEHICLEDATA_SPEED,1
VEHICLEDATA_FUELLEVEL,2
VEHICLEDATA_FUELECONOMY,3
VEHICLEDATA_ENGINERPM,4
VEHICLEDATA_BATTVOLTS,5
VEHICLEDATA_RAINSENSOR,6
VEHICLEDATA_ODOMETER,7
VEHICLEDATA_VIN,8
VEHICLEDATA_EXTERNTEMP,9
VEHICLEDATA_PRNDLSTATUS,10
VEHICLEDATA_TIREPRESSURE,11
VEHICLEDATA_BATTERYPACKVOLTAGE,12
VEHICLEDATA_BATTERYCURRENT,13
VEHICLEDATA_BATTERYTEMPERATURE,14
VEHICLEDATA_SATESN,15