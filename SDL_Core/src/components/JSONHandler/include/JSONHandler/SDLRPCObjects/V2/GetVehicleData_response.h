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

#ifndef NSSMARTDEVICELINKKRPCV2_GETVEHICLEDATA_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_GETVEHICLEDATA_RESPONSE_INCLUDE

#include <string>

#include "GPSData.h"
#include "PRNDL.h"
#include "TireStatus.h"
#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPCV2
{
    class GetVehicleData_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        GetVehicleData_response(const GetVehicleData_response& c);
        GetVehicleData_response(void);

        virtual ~GetVehicleData_response(void);

        GetVehicleData_response& operator =(const GetVehicleData_response&);

        bool checkIntegrity(void);

        const GPSData* get_gps(void) const;
        const float* get_speed(void) const;
        const unsigned int* get_rpm(void) const;
        const float* get_fuelLevel(void) const;
        const float* get_avgFuelEconomy(void) const;
        const float* get_batteryVoltage(void) const;
        const float* get_externalTemperature(void) const;
        const std::string* get_vin(void) const;
        const PRNDL* get_prndl(void) const;
        const TireStatus* get_tirePressure(void) const;
        const float* get_batteryPackVoltage(void) const;
        const float* get_batteryPackCurrent(void) const;
        const int* get_batteryPackTemperature(void) const;
        const int* get_engineTorque(void) const;
        const unsigned int* get_odometer(void) const;
        const unsigned int* get_tripOdometer(void) const;
        const std::string* get_satRadioESN(void) const;

        void reset_gps(void);
        bool set_gps(const GPSData& gps_);
        void reset_speed(void);
        bool set_speed(float speed_);
        void reset_rpm(void);
        bool set_rpm(unsigned int rpm_);
        void reset_fuelLevel(void);
        bool set_fuelLevel(float fuelLevel_);
        void reset_avgFuelEconomy(void);
        bool set_avgFuelEconomy(float avgFuelEconomy_);
        void reset_batteryVoltage(void);
        bool set_batteryVoltage(float batteryVoltage_);
        void reset_externalTemperature(void);
        bool set_externalTemperature(float externalTemperature_);
        void reset_vin(void);
        bool set_vin(const std::string& vin_);
        void reset_prndl(void);
        bool set_prndl(const PRNDL& prndl_);
        void reset_tirePressure(void);
        bool set_tirePressure(const TireStatus& tirePressure_);
        void reset_batteryPackVoltage(void);
        bool set_batteryPackVoltage(float batteryPackVoltage_);
        void reset_batteryPackCurrent(void);
        bool set_batteryPackCurrent(float batteryPackCurrent_);
        void reset_batteryPackTemperature(void);
        bool set_batteryPackTemperature(int batteryPackTemperature_);
        void reset_engineTorque(void);
        bool set_engineTorque(int engineTorque_);
        void reset_odometer(void);
        bool set_odometer(unsigned int odometer_);
        void reset_tripOdometer(void);
        bool set_tripOdometer(unsigned int tripOdometer_);
        void reset_satRadioESN(void);
        bool set_satRadioESN(const std::string& satRadioESN_);

    private:
        friend class GetVehicleData_responseMarshaller;

        /**
             See GPSData
             Subscribable
        */
        GPSData* gps;

        /**
             The vehicle speed in kilometers per hour
             Subscribable
        */
        float* speed; //!<  (-100,500)

        /**
             The number of revolutions per minute of the engine
             Subscribable
        */
        unsigned int* rpm;    //!<  (0,20000)

        ///  The fuel level
        float* fuelLevel; //!<  (0,100)

        ///  The average fuel economy in litres/100km
        float* avgFuelEconomy;    //!<  (0,7000)

        ///  The voltage in Volts
        float* batteryVoltage;    //!<  (0,26)

        ///  The external temperature in degrees celsius.
        float* externalTemperature;   //!<  (-40,100)

        ///  Vehicle identification number.
        std::string* vin; //!< (17)

        /**
             See PRNDL.
             Subscribable
        */
        PRNDL* prndl;

        /**
             See TireStatus.
             Subscribable
        */
        TireStatus* tirePressure;

        ///  Battery pack voltage of hybrid and electrical vehicles in volts
        float* batteryPackVoltage;    //!<  (-1000,1000)

        ///  Battery current of hybrid and electrical vehicles in amperes
        float* batteryPackCurrent;    //!<  (-1000,1000)

        ///  Battery temperature of hybrid and electrical vehicles in degrees celsius
        int* batteryPackTemperature;  //!<  (-50,250)

        /**
             Engine torque in Nm
             Subscribable
        */
        int* engineTorque;    //!<  (-10000,55000)

        ///  Odometer in km
        unsigned int* odometer;   //!<  (0,2000000)

        ///  Odometer of trip in km
        unsigned int* tripOdometer;   //!<  (0,2000000)

        ///  The hexadecimal ESN of the satellite radio (if supported).
        std::string* satRadioESN; //!< (500)
    };
}

#endif
