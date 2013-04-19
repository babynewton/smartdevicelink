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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Slider.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SliderMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;

bool SliderMarshaller::checkIntegrity(Slider& s)
{
  return checkIntegrityConst(s);
}


bool SliderMarshaller::fromString(const std::string& s,Slider& e)
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


const std::string SliderMarshaller::toString(const Slider& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SliderMarshaller::checkIntegrityConst(const Slider& s)
{
  if(s.numTicks<2 || s.numTicks>26)  return false;

  if(s.position<1 || s.position>26)  return false;

  if(s.sliderHeader.length()>500)  return false;

  if(s.sliderFooter)
  {
    unsigned int i=s.sliderFooter[0].size();
    if(i<1)  return false;
    if(i>26)  return false;
    while(i--)
    {
      if(s.sliderFooter[0][i].length()>500)  return false;
    }
  }

  if(s.timeout>65535)  return false;

  return true;
}


Json::Value SliderMarshaller::toJSON(const Slider& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.Slider");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["numTicks"]=Json::Value(e.numTicks);;
  json["params"]["position"]=Json::Value(e.position);;
  json["params"]["sliderHeader"]=Json::Value(e.sliderHeader);;
  if(e.sliderFooter)
  {
    unsigned int i=e.sliderFooter[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.sliderFooter[0][i]);

    json["params"]["sliderFooter"]=j;
  }
  json["params"]["timeout"]=Json::Value(e.timeout);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool SliderMarshaller::fromJSON(const Json::Value& json,Slider& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.Slider"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("numTicks") || !js["numTicks"].isInt())  return false;
    c.numTicks=js["numTicks"].asInt();
    if(c.numTicks<2 || c.numTicks>26)  return false;

    if(!js.isMember("position") || !js["position"].isInt())  return false;
    c.position=js["position"].asInt();
    if(c.position<1 || c.position>16)  return false;

    if(!js.isMember("sliderHeader") || !js["sliderHeader"].isString())  return false;
    c.sliderHeader=js["sliderHeader"].asString();
    if(c.sliderHeader.length()>500)  return false;

    if(c.sliderFooter)  delete c.sliderFooter;
    c.sliderFooter=0;
    if(js.isMember("sliderFooter"))
    {
      if(!js["sliderFooter"].isArray()) return false;
      unsigned int i=js["sliderFooter"].size();
      if(i<1)  return false;
      if(i>26)  return false;

      c.sliderFooter=new std::vector<std::string>();
      c.sliderFooter->resize(js["sliderFooter"].size());

      while(i--)
      {
        if(!js["sliderFooter"][i].isString())
          return false;

        c.sliderFooter[0][i]=js["sliderFooter"][i].asString();
        if(c.sliderFooter[0][i].length()>500)  return false;
      }
    }


    if(!js.isMember("timeout") || !js["timeout"].isInt())  return false;
    c.timeout=js["timeout"].asInt();
    if(c.timeout>65535)  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
