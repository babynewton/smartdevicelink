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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/OnVehicleData.h"
#include "../src/SDLRPCObjectsImpl/V2/GPSDataMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/PRNDLMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/TireStatusMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/OnVehicleDataMarshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;

bool OnVehicleDataMarshaller::checkIntegrity(OnVehicleData& s)
{
  return checkIntegrityConst(s);
}


bool OnVehicleDataMarshaller::fromString(const std::string& s,OnVehicleData& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string OnVehicleDataMarshaller::toString(const OnVehicleData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnVehicleDataMarshaller::checkIntegrityConst(const OnVehicleData& s)
{
  if(s.gps && (!NsSmartDeviceLinkRPCV2::GPSDataMarshaller::checkIntegrityConst(s.gps[0])))  return false;

  if(s.speed && (s.speed[0]<-100 || s.speed[0]>500))  return false;

  if(s.rpm && (s.rpm[0]>20000))  return false;

  if(s.fuelLevel && (s.fuelLevel[0]<0 || s.fuelLevel[0]>100))  return false;

  if(s.avgFuelEconomy && (s.avgFuelEconomy[0]<0 || s.avgFuelEconomy[0]>7000))  return false;

  if(s.batteryVoltage && (s.batteryVoltage[0]<0 || s.batteryVoltage[0]>26))  return false;

  if(s.externalTemperature && (s.externalTemperature[0]<-40 || s.externalTemperature[0]>100))  return false;

  if(s.vin && (s.vin[0].length()>17))  return false;

  if(s.prndl && (!NsSmartDeviceLinkRPCV2::PRNDLMarshaller::checkIntegrityConst(s.prndl[0])))  return false;

  if(s.tirePressure && (!NsSmartDeviceLinkRPCV2::TireStatusMarshaller::checkIntegrityConst(s.tirePressure[0])))  return false;

  if(s.batteryPackVoltage && (s.batteryPackVoltage[0]<-1000 || s.batteryPackVoltage[0]>1000))  return false;

  if(s.batteryPackCurrent && (s.batteryPackCurrent[0]<-1000 || s.batteryPackCurrent[0]>1000))  return false;

  if(s.batteryPackTemperature && (s.batteryPackTemperature[0]<-50 || s.batteryPackTemperature[0]>250))  return false;

  if(s.engineTorque && (s.engineTorque[0]<-10000 || s.engineTorque[0]>55000))  return false;

  if(s.odometer && (s.odometer[0]>2000000))  return false;

  if(s.tripOdometer && (s.tripOdometer[0]>2000000))  return false;

  if(s.genericbinary && (s.genericbinary[0].length()>500))  return false;

  return true;
}


Json::Value OnVehicleDataMarshaller::toJSON(const OnVehicleData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("VehicleInfo.OnVehicleData");
  json["params"]=Json::Value(Json::objectValue);

  if(e.gps)
    json["params"]["gps"]=NsSmartDeviceLinkRPCV2::GPSDataMarshaller::toJSON(e.gps[0]);;
  if(e.speed)
    json["params"]["speed"]=Json::Value(e.speed[0]);;
  if(e.rpm)
    json["params"]["rpm"]=Json::Value(e.rpm[0]);;
  if(e.fuelLevel)
    json["params"]["fuelLevel"]=Json::Value(e.fuelLevel[0]);;
  if(e.avgFuelEconomy)
    json["params"]["avgFuelEconomy"]=Json::Value(e.avgFuelEconomy[0]);;
  if(e.batteryVoltage)
    json["params"]["batteryVoltage"]=Json::Value(e.batteryVoltage[0]);;
  if(e.externalTemperature)
    json["params"]["externalTemperature"]=Json::Value(e.externalTemperature[0]);;
  if(e.vin)
    json["params"]["vin"]=Json::Value(e.vin[0]);;
  if(e.prndl)
    json["params"]["prndl"]=NsSmartDeviceLinkRPCV2::PRNDLMarshaller::toJSON(e.prndl[0]);;
  if(e.tirePressure)
    json["params"]["tirePressure"]=NsSmartDeviceLinkRPCV2::TireStatusMarshaller::toJSON(e.tirePressure[0]);;
  if(e.batteryPackVoltage)
    json["params"]["batteryPackVoltage"]=Json::Value(e.batteryPackVoltage[0]);;
  if(e.batteryPackCurrent)
    json["params"]["batteryPackCurrent"]=Json::Value(e.batteryPackCurrent[0]);;
  if(e.batteryPackTemperature)
    json["params"]["batteryPackTemperature"]=Json::Value(e.batteryPackTemperature[0]);;
  if(e.engineTorque)
    json["params"]["engineTorque"]=Json::Value(e.engineTorque[0]);;
  if(e.odometer)
    json["params"]["odometer"]=Json::Value(e.odometer[0]);;
  if(e.tripOdometer)
    json["params"]["tripOdometer"]=Json::Value(e.tripOdometer[0]);;
  if(e.genericbinary)
    json["params"]["genericbinary"]=Json::Value(e.genericbinary[0]);;
  return json;
}


bool OnVehicleDataMarshaller::fromJSON(const Json::Value& json,OnVehicleData& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("VehicleInfo.OnVehicleData"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(c.gps)  delete c.gps;
    c.gps=0;
    if(js.isMember("gps"))
    {
      c.gps=new NsSmartDeviceLinkRPCV2::GPSData();
      if(!NsSmartDeviceLinkRPCV2::GPSDataMarshaller::fromJSON(js["gps"],c.gps[0]))  return false;
    }

    if(c.speed)  delete c.speed;
    c.speed=0;
    if(js.isMember("speed"))
    {
      if(!js["speed"].isNumeric())  return false;
      c.speed=new float();
      c.speed[0]=js["speed"].asDouble();
      if(c.speed[0]<-100 || c.speed[0]>500)  return false;

    }

    if(c.rpm)  delete c.rpm;
    c.rpm=0;
    if(js.isMember("rpm"))
    {
      if(!js["rpm"].isInt())  return false;
      c.rpm=new unsigned int();
      c.rpm[0]=js["rpm"].asInt();
      if(c.rpm[0]>20000)  return false;

    }

    if(c.fuelLevel)  delete c.fuelLevel;
    c.fuelLevel=0;
    if(js.isMember("fuelLevel"))
    {
      if(!js["fuelLevel"].isNumeric())  return false;
      c.fuelLevel=new float();
      c.fuelLevel[0]=js["fuelLevel"].asDouble();
      if(c.fuelLevel[0]<0 || c.fuelLevel[0]>100)  return false;

    }

    if(c.avgFuelEconomy)  delete c.avgFuelEconomy;
    c.avgFuelEconomy=0;
    if(js.isMember("avgFuelEconomy"))
    {
      if(!js["avgFuelEconomy"].isNumeric())  return false;
      c.avgFuelEconomy=new float();
      c.avgFuelEconomy[0]=js["avgFuelEconomy"].asDouble();
      if(c.avgFuelEconomy[0]<0 || c.avgFuelEconomy[0]>7000)  return false;

    }

    if(c.batteryVoltage)  delete c.batteryVoltage;
    c.batteryVoltage=0;
    if(js.isMember("batteryVoltage"))
    {
      if(!js["batteryVoltage"].isNumeric())  return false;
      c.batteryVoltage=new float();
      c.batteryVoltage[0]=js["batteryVoltage"].asDouble();
      if(c.batteryVoltage[0]<0 || c.batteryVoltage[0]>26)  return false;

    }

    if(c.externalTemperature)  delete c.externalTemperature;
    c.externalTemperature=0;
    if(js.isMember("externalTemperature"))
    {
      if(!js["externalTemperature"].isNumeric())  return false;
      c.externalTemperature=new float();
      c.externalTemperature[0]=js["externalTemperature"].asDouble();
      if(c.externalTemperature[0]<-40 || c.externalTemperature[0]>100)  return false;

    }

    if(c.vin)  delete c.vin;
    c.vin=0;
    if(js.isMember("vin"))
    {
      if(!js["vin"].isString())  return false;
      c.vin=new std::string();
      c.vin[0]=js["vin"].asString();
      if(c.vin[0].length()>17)  return false;

    }

    if(c.prndl)  delete c.prndl;
    c.prndl=0;
    if(js.isMember("prndl"))
    {
      c.prndl=new NsSmartDeviceLinkRPCV2::PRNDL();
      if(!NsSmartDeviceLinkRPCV2::PRNDLMarshaller::fromJSON(js["prndl"],c.prndl[0]))  return false;
    }

    if(c.tirePressure)  delete c.tirePressure;
    c.tirePressure=0;
    if(js.isMember("tirePressure"))
    {
      c.tirePressure=new NsSmartDeviceLinkRPCV2::TireStatus();
      if(!NsSmartDeviceLinkRPCV2::TireStatusMarshaller::fromJSON(js["tirePressure"],c.tirePressure[0]))  return false;
    }

    if(c.batteryPackVoltage)  delete c.batteryPackVoltage;
    c.batteryPackVoltage=0;
    if(js.isMember("batteryPackVoltage"))
    {
      if(!js["batteryPackVoltage"].isNumeric())  return false;
      c.batteryPackVoltage=new float();
      c.batteryPackVoltage[0]=js["batteryPackVoltage"].asDouble();
      if(c.batteryPackVoltage[0]<-1000 || c.batteryPackVoltage[0]>1000)  return false;

    }

    if(c.batteryPackCurrent)  delete c.batteryPackCurrent;
    c.batteryPackCurrent=0;
    if(js.isMember("batteryPackCurrent"))
    {
      if(!js["batteryPackCurrent"].isNumeric())  return false;
      c.batteryPackCurrent=new float();
      c.batteryPackCurrent[0]=js["batteryPackCurrent"].asDouble();
      if(c.batteryPackCurrent[0]<-1000 || c.batteryPackCurrent[0]>1000)  return false;

    }

    if(c.batteryPackTemperature)  delete c.batteryPackTemperature;
    c.batteryPackTemperature=0;
    if(js.isMember("batteryPackTemperature"))
    {
      if(!js["batteryPackTemperature"].isInt())  return false;
      c.batteryPackTemperature=new int();
      c.batteryPackTemperature[0]=js["batteryPackTemperature"].asInt();
      if(c.batteryPackTemperature[0]<-50 || c.batteryPackTemperature[0]>250)  return false;

    }

    if(c.engineTorque)  delete c.engineTorque;
    c.engineTorque=0;
    if(js.isMember("engineTorque"))
    {
      if(!js["engineTorque"].isInt())  return false;
      c.engineTorque=new int();
      c.engineTorque[0]=js["engineTorque"].asInt();
      if(c.engineTorque[0]<-10000 || c.engineTorque[0]>55000)  return false;

    }

    if(c.odometer)  delete c.odometer;
    c.odometer=0;
    if(js.isMember("odometer"))
    {
      if(!js["odometer"].isInt())  return false;
      c.odometer=new unsigned int();
      c.odometer[0]=js["odometer"].asInt();
      if(c.odometer[0]>2000000)  return false;

    }

    if(c.tripOdometer)  delete c.tripOdometer;
    c.tripOdometer=0;
    if(js.isMember("tripOdometer"))
    {
      if(!js["tripOdometer"].isInt())  return false;
      c.tripOdometer=new unsigned int();
      c.tripOdometer[0]=js["tripOdometer"].asInt();
      if(c.tripOdometer[0]>2000000)  return false;

    }

    if(c.genericbinary)  delete c.genericbinary;
    c.genericbinary=0;
    if(js.isMember("genericbinary"))
    {
      if(!js["genericbinary"].isString())  return false;
      c.genericbinary=new std::string();
      c.genericbinary[0]=js["genericbinary"].asString();
      if(c.genericbinary[0].length()>500)  return false;

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
