//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_ONVEHICLEDATA_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_ONVEHICLEDATA_INCLUDE

#include <string>
#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/GPSData.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/PRNDL.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/TireStatus.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    class OnVehicleData : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnVehicleData(const OnVehicleData& c);
      OnVehicleData(void);
    
      OnVehicleData& operator =(const OnVehicleData&);
    
      virtual ~OnVehicleData(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsSmartDeviceLinkRPCV2::GPSData* get_gps(void);
      const float* get_speed(void);
      const unsigned int* get_rpm(void);
      const float* get_fuelLevel(void);
      const float* get_avgFuelEconomy(void);
      const float* get_batteryVoltage(void);
      const float* get_externalTemperature(void);
      const std::string* get_vin(void);
      const NsSmartDeviceLinkRPCV2::PRNDL* get_prndl(void);
      const NsSmartDeviceLinkRPCV2::TireStatus* get_tirePressure(void);
      const float* get_batteryPackVoltage(void);
      const float* get_batteryPackCurrent(void);
      const int* get_batteryPackTemperature(void);
      const int* get_engineTorque(void);
      const unsigned int* get_odometer(void);
      const unsigned int* get_tripOdometer(void);
      const std::string* get_genericbinary(void);

// setters
      bool set_gps(const NsSmartDeviceLinkRPCV2::GPSData& gps);

      void reset_gps(void);

/// -100 <= speed <= 500
      bool set_speed(const float& speed);

      void reset_speed(void);

/// rpm <= 20000
      bool set_rpm(const unsigned int& rpm);

      void reset_rpm(void);

/// 0 <= fuelLevel <= 100
      bool set_fuelLevel(const float& fuelLevel);

      void reset_fuelLevel(void);

/// 0 <= avgFuelEconomy <= 7000
      bool set_avgFuelEconomy(const float& avgFuelEconomy);

      void reset_avgFuelEconomy(void);

/// 0 <= batteryVoltage <= 26
      bool set_batteryVoltage(const float& batteryVoltage);

      void reset_batteryVoltage(void);

/// -40 <= externalTemperature <= 100
      bool set_externalTemperature(const float& externalTemperature);

      void reset_externalTemperature(void);

/// vin <= 17
      bool set_vin(const std::string& vin);

      void reset_vin(void);

      bool set_prndl(const NsSmartDeviceLinkRPCV2::PRNDL& prndl);

      void reset_prndl(void);

      bool set_tirePressure(const NsSmartDeviceLinkRPCV2::TireStatus& tirePressure);

      void reset_tirePressure(void);

/// -1000 <= batteryPackVoltage <= 1000
      bool set_batteryPackVoltage(const float& batteryPackVoltage);

      void reset_batteryPackVoltage(void);

/// -1000 <= batteryPackCurrent <= 1000
      bool set_batteryPackCurrent(const float& batteryPackCurrent);

      void reset_batteryPackCurrent(void);

/// -50 <= batteryPackTemperature <= 250
      bool set_batteryPackTemperature(const int& batteryPackTemperature);

      void reset_batteryPackTemperature(void);

/// -10000 <= engineTorque <= 55000
      bool set_engineTorque(const int& engineTorque);

      void reset_engineTorque(void);

/// odometer <= 2000000
      bool set_odometer(const unsigned int& odometer);

      void reset_odometer(void);

/// tripOdometer <= 2000000
      bool set_tripOdometer(const unsigned int& tripOdometer);

      void reset_tripOdometer(void);

/// genericbinary <= 500
      bool set_genericbinary(const std::string& genericbinary);

      void reset_genericbinary(void);


    private:

      friend class OnVehicleDataMarshaller;

      NsSmartDeviceLinkRPCV2::GPSData* gps;
      float* speed;
      unsigned int* rpm;
      float* fuelLevel;
      float* avgFuelEconomy;
      float* batteryVoltage;
      float* externalTemperature;
      std::string* vin;
      NsSmartDeviceLinkRPCV2::PRNDL* prndl;
      NsSmartDeviceLinkRPCV2::TireStatus* tirePressure;
      float* batteryPackVoltage;
      float* batteryPackCurrent;
      int* batteryPackTemperature;
      int* engineTorque;
      unsigned int* odometer;
      unsigned int* tripOdometer;
      std::string* genericbinary;

    };
  }
}

#endif
