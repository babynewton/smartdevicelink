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

#include "../include/JSONHandler/SDLRPCObjects/V2/Slider_request.h"
#include "Slider_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"

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
Slider_request& Slider_request::operator =(const Slider_request& c)
{
  numTicks= c.numTicks;
  position= c.position;
  sliderHeader= c.sliderHeader;
  sliderFooter= c.sliderFooter ? new std::vector<std::string>(c.sliderFooter[0]) : 0;
  timeout= c.timeout;

  return *this;
}


Slider_request::~Slider_request(void)
{
  if(sliderFooter)
    delete sliderFooter;
}


Slider_request::Slider_request(const Slider_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool Slider_request::checkIntegrity(void)
{
  return Slider_requestMarshaller::checkIntegrity(*this);
}


Slider_request::Slider_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      sliderFooter(0)
{
}



bool Slider_request::set_numTicks(unsigned int numTicks_)
{
  if(numTicks_>26)  return false;
  if(numTicks_<2)  return false;
  numTicks=numTicks_;
  return true;
}

bool Slider_request::set_position(unsigned int position_)
{
  if(position_>26)  return false;
  if(position_<1)  return false;
  position=position_;
  return true;
}

bool Slider_request::set_sliderHeader(const std::string& sliderHeader_)
{
  if(sliderHeader_.length()>500)  return false;
  sliderHeader=sliderHeader_;
  return true;
}

bool Slider_request::set_sliderFooter(const std::vector<std::string>& sliderFooter_)
{
  unsigned int i=sliderFooter_.size();
  if(i>26 || i<1)  return false;
  while(i--)
  {
    if(sliderFooter_[i].length()>500)  return false;
  }
  delete sliderFooter;
  sliderFooter=0;

  sliderFooter=new std::vector<std::string>(sliderFooter_);
  return true;
}

void Slider_request::reset_sliderFooter(void)
{
  if(sliderFooter)
    delete sliderFooter;
  sliderFooter=0;
}

bool Slider_request::set_timeout(unsigned int timeout_)
{
  if(timeout_>65535)  return false;
  timeout=timeout_;
  return true;
}




unsigned int Slider_request::get_numTicks(void) const
{
  return numTicks;
}

unsigned int Slider_request::get_position(void) const
{
  return position;
}

const std::string& Slider_request::get_sliderHeader(void) const 
{
  return sliderHeader;
}

const std::vector<std::string>* Slider_request::get_sliderFooter(void) const 
{
  return sliderFooter;
}

unsigned int Slider_request::get_timeout(void) const
{
  return timeout;
}

