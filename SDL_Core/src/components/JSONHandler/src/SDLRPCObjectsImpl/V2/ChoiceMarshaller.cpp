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

#include "../include/JSONHandler/SDLRPCObjects/V2/Choice.h"
#include "ImageMarshaller.h"

#include "ChoiceMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


bool ChoiceMarshaller::checkIntegrity(Choice& s)
{
  return checkIntegrityConst(s);
}


bool ChoiceMarshaller::fromString(const std::string& s,Choice& e)
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


const std::string ChoiceMarshaller::toString(const Choice& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ChoiceMarshaller::checkIntegrityConst(const Choice& s)
{
  if(s.choiceID>65535)  return false;

  // TODO(AK): Quick fix for HMI communication with app(v1)
  if (s.image.get_imageType().get() != NsSmartDeviceLinkRPCV2::ImageType::INVALID_ENUM)
    if(!ImageMarshaller::checkIntegrityConst(s.image))  return false;

  if(s.menuName.length()>500)  return false;
  {
    unsigned int i=s.vrCommands.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
      if(s.vrCommands[i].length()>99)  return false;
    }
  }
  return true;
}

Json::Value ChoiceMarshaller::toJSON(const Choice& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["choiceID"]=Json::Value(e.choiceID);

  // TODO(AK): Quick fix for HMI communication with app(v1)
  if (e.image.get_imageType().get() != NsSmartDeviceLinkRPCV2::ImageType::INVALID_ENUM)
    json["image"]=ImageMarshaller::toJSON(e.image);

  json["menuName"]=Json::Value(e.menuName);

  json["vrCommands"]=Json::Value(Json::arrayValue);
  json["vrCommands"].resize(e.vrCommands.size());
  for(unsigned int i=0;i<e.vrCommands.size();i++)
    json["vrCommands"][i]=Json::Value(e.vrCommands[i]);


  return json;
}


bool ChoiceMarshaller::fromJSON(const Json::Value& json,Choice& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("choiceID"))  return false;
    {
      const Json::Value& j=json["choiceID"];
      if(!j.isInt())  return false;
      c.choiceID=j.asInt();
    }
    if(json.isMember("image"))
    {
      const Json::Value& j=json["image"];
      if(!ImageMarshaller::fromJSON(j,c.image))
        return false;
    }
    if(!json.isMember("menuName"))  return false;
    {
      const Json::Value& j=json["menuName"];
      if(!j.isString())  return false;
      c.menuName=j.asString();
    }
    if(!json.isMember("vrCommands"))  return false;
    {
      const Json::Value& j=json["vrCommands"];
      if(!j.isArray())  return false;
      c.vrCommands.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.vrCommands[i]=j[i].asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

