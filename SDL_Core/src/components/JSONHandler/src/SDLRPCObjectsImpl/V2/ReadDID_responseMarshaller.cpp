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

#include "../include/JSONHandler/SDLRPCObjects/V2/ReadDID_response.h"
#include "ResultMarshaller.h"
#include "VehicleDataResultCodeMarshaller.h"

#include "ReadDID_responseMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool ReadDID_responseMarshaller::checkIntegrity(ReadDID_response& s)
{
  return checkIntegrityConst(s);
}


bool ReadDID_responseMarshaller::fromString(const std::string& s,ReadDID_response& e)
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


const std::string ReadDID_responseMarshaller::toString(const ReadDID_response& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ReadDID_responseMarshaller::checkIntegrityConst(const ReadDID_response& s)
{
  if(!ResultMarshaller::checkIntegrityConst(s.resultCode))  return false;
  if(s.info && s.info->length()>1000)  return false;
  if(s.dataResult)
  {
    unsigned int i=s.dataResult[0].size();
    if(i>1000 || i<0)  return false;
    while(i--)
    {
    if(!VehicleDataResultCodeMarshaller::checkIntegrityConst(s.dataResult[0][i]))   return false;
    }
  }
  if(s.data)
  {
    unsigned int i=s.data[0].size();
    if(i>1000 || i<0)  return false;
    while(i--)
    {
      if(s.data[0][i].length()>5000)  return false;
    }
  }
  return true;
}

Json::Value ReadDID_responseMarshaller::toJSON(const ReadDID_response& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["success"]=Json::Value(e.success);

  json["resultCode"]=ResultMarshaller::toJSON(e.resultCode);

  if(e.info)
    json["info"]=Json::Value(*e.info);

  if(e.dataResult)
  {
    unsigned int sz=e.dataResult->size();
    json["dataResult"]=Json::Value(Json::arrayValue);
    json["dataResult"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["dataResult"][i]=VehicleDataResultCodeMarshaller::toJSON(e.dataResult[0][i]);
  }

  if(e.data)
  {
    unsigned int sz=e.data->size();
    json["data"]=Json::Value(Json::arrayValue);
    json["data"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      json["data"][i]=Json::Value(e.data[0][i]);
  }

  return json;
}


bool ReadDID_responseMarshaller::fromJSON(const Json::Value& json,ReadDID_response& c)
{
  if(c.info)  delete c.info;
  c.info=0;

  if(c.dataResult)  delete c.dataResult;
  c.dataResult=0;

  if(c.data)  delete c.data;
  c.data=0;

  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("success"))  return false;
    {
      const Json::Value& j=json["success"];
      if(!j.isBool())  return false;
      c.success=j.asBool();
    }
    if(!json.isMember("resultCode"))  return false;
    {
      const Json::Value& j=json["resultCode"];
      if(!ResultMarshaller::fromJSON(j,c.resultCode))
        return false;
    }
    if(json.isMember("info"))
    {
      const Json::Value& j=json["info"];
      if(!j.isString())  return false;
      c.info=new std::string(j.asString());
    }
    if(json.isMember("dataResult"))
    {
      const Json::Value& j=json["dataResult"];
      if(!j.isArray())  return false;
      c.dataResult=new std::vector<VehicleDataResultCode>();
      c.dataResult->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        VehicleDataResultCode t;
        if(!VehicleDataResultCodeMarshaller::fromJSON(j[i],t))
          return false;
        c.dataResult[0][i]=t;
      }

    }
    if(json.isMember("data"))
    {
      const Json::Value& j=json["data"];
      if(!j.isArray())  return false;
      c.data=new std::vector<std::string>();
      c.data->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.data[0][i]=j[i].asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

