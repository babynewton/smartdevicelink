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

#include "../include/JSONHandler/SDLRPCObjects/V2/ShowConstantTBT_request.h"
#include "ShowConstantTBT_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "ImageMarshaller.h"
#include "SoftButtonMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;
ShowConstantTBT_request& ShowConstantTBT_request::operator =(const ShowConstantTBT_request& c)
{
  navigationText1= c.navigationText1 ? new std::string(c.navigationText1[0]) : 0;
  navigationText2= c.navigationText2 ? new std::string(c.navigationText2[0]) : 0;
  eta= c.eta ? new std::string(c.eta[0]) : 0;
  totalDistance= c.totalDistance ? new std::string(c.totalDistance[0]) : 0;
  turnIcon= c.turnIcon;
  distanceToManeuver= c.distanceToManeuver;
  distanceToManeuverScale= c.distanceToManeuverScale;
  maneuverComplete= c.maneuverComplete ? new bool(c.maneuverComplete[0]) : 0;
  softButtons= c.softButtons;

  return *this;
}


ShowConstantTBT_request::~ShowConstantTBT_request(void)
{
  if(navigationText1)
    delete navigationText1;
  if(navigationText2)
    delete navigationText2;
  if(eta)
    delete eta;
  if(totalDistance)
    delete totalDistance;
  if(maneuverComplete)
    delete maneuverComplete;
}


ShowConstantTBT_request::ShowConstantTBT_request(const ShowConstantTBT_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool ShowConstantTBT_request::checkIntegrity(void)
{
  return ShowConstantTBT_requestMarshaller::checkIntegrity(*this);
}


ShowConstantTBT_request::ShowConstantTBT_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      navigationText1(0),
    navigationText2(0),
    eta(0),
    totalDistance(0),
    maneuverComplete(0)
{
}



bool ShowConstantTBT_request::set_navigationText1(const std::string& navigationText1_)
{
  if(navigationText1_.length()>500)  return false;
  delete navigationText1;
  navigationText1=0;

  navigationText1=new std::string(navigationText1_);
  return true;
}

void ShowConstantTBT_request::reset_navigationText1(void)
{
  if(navigationText1)
    delete navigationText1;
  navigationText1=0;
}

bool ShowConstantTBT_request::set_navigationText2(const std::string& navigationText2_)
{
  if(navigationText2_.length()>500)  return false;
  delete navigationText2;
  navigationText2=0;

  navigationText2=new std::string(navigationText2_);
  return true;
}

void ShowConstantTBT_request::reset_navigationText2(void)
{
  if(navigationText2)
    delete navigationText2;
  navigationText2=0;
}

bool ShowConstantTBT_request::set_eta(const std::string& eta_)
{
  if(eta_.length()>500)  return false;
  delete eta;
  eta=0;

  eta=new std::string(eta_);
  return true;
}

void ShowConstantTBT_request::reset_eta(void)
{
  if(eta)
    delete eta;
  eta=0;
}

bool ShowConstantTBT_request::set_totalDistance(const std::string& totalDistance_)
{
  if(totalDistance_.length()>500)  return false;
  delete totalDistance;
  totalDistance=0;

  totalDistance=new std::string(totalDistance_);
  return true;
}

void ShowConstantTBT_request::reset_totalDistance(void)
{
  if(totalDistance)
    delete totalDistance;
  totalDistance=0;
}

bool ShowConstantTBT_request::set_turnIcon(const Image& turnIcon_)
{
  if(!ImageMarshaller::checkIntegrityConst(turnIcon_))   return false;
  turnIcon=turnIcon_;
  return true;
}

bool ShowConstantTBT_request::set_distanceToManeuver(float distanceToManeuver_)
{
  if(distanceToManeuver_>1000000000)  return false;
  if(distanceToManeuver_<0)  return false;
  distanceToManeuver=distanceToManeuver_;
  return true;
}

bool ShowConstantTBT_request::set_distanceToManeuverScale(float distanceToManeuverScale_)
{
  if(distanceToManeuverScale_>1000000000)  return false;
  if(distanceToManeuverScale_<0)  return false;
  distanceToManeuverScale=distanceToManeuverScale_;
  return true;
}

bool ShowConstantTBT_request::set_maneuverComplete(bool maneuverComplete_)
{
  delete maneuverComplete;
  maneuverComplete=0;

  maneuverComplete=new bool(maneuverComplete_);
  return true;
}

void ShowConstantTBT_request::reset_maneuverComplete(void)
{
  if(maneuverComplete)
    delete maneuverComplete;
  maneuverComplete=0;
}

bool ShowConstantTBT_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>3 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  softButtons=softButtons_;
  return true;
}




const std::string* ShowConstantTBT_request::get_navigationText1(void) const 
{
  return navigationText1;
}

const std::string* ShowConstantTBT_request::get_navigationText2(void) const 
{
  return navigationText2;
}

const std::string* ShowConstantTBT_request::get_eta(void) const 
{
  return eta;
}

const std::string* ShowConstantTBT_request::get_totalDistance(void) const 
{
  return totalDistance;
}

const Image& ShowConstantTBT_request::get_turnIcon(void) const 
{
  return turnIcon;
}

float ShowConstantTBT_request::get_distanceToManeuver(void) const
{
  return distanceToManeuver;
}

float ShowConstantTBT_request::get_distanceToManeuverScale(void) const
{
  return distanceToManeuverScale;
}

const bool* ShowConstantTBT_request::get_maneuverComplete(void) const 
{
  return maneuverComplete;
}

const std::vector<SoftButton>& ShowConstantTBT_request::get_softButtons(void) const 
{
  return softButtons;
}

