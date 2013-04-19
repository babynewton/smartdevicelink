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

#include "../include/JSONHandler/SDLRPCObjects/V1/Show_request.h"
#include "Show_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"
#include "TextAlignmentMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;
Show_request& Show_request::operator =(const Show_request& c)
{
  mainField1= c.mainField1 ? new std::string(c.mainField1[0]) : 0;
  mainField2= c.mainField2 ? new std::string(c.mainField2[0]) : 0;
  alignment= c.alignment ? new TextAlignment(c.alignment[0]) : 0;
  statusBar= c.statusBar ? new std::string(c.statusBar[0]) : 0;
  mediaClock= c.mediaClock ? new std::string(c.mediaClock[0]) : 0;
  mediaTrack= c.mediaTrack ? new std::string(c.mediaTrack[0]) : 0;

  return *this;}


Show_request::~Show_request(void)
{
  if(mainField1)
    delete mainField1;
  if(mainField2)
    delete mainField2;
  if(alignment)
    delete alignment;
  if(statusBar)
    delete statusBar;
  if(mediaClock)
    delete mediaClock;
  if(mediaTrack)
    delete mediaTrack;
}


Show_request::Show_request(const Show_request& c)
{
  *this=c;
}


bool Show_request::checkIntegrity(void)
{
  return Show_requestMarshaller::checkIntegrity(*this);
}


Show_request::Show_request(void) : SDLRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SHOW_REQUEST),
      mainField1(0),
    mainField2(0),
    alignment(0),
    statusBar(0),
    mediaClock(0),
    mediaTrack(0)
{
}



bool Show_request::set_mainField1(const std::string& mainField1_)
{
  if(mainField1_.length()>500)  return false;
  delete mainField1;
  mainField1=0;

  mainField1=new std::string(mainField1_);
  return true;
}

void Show_request::reset_mainField1(void)
{
  if(mainField1)
    delete mainField1;
  mainField1=0;
}

bool Show_request::set_mainField2(const std::string& mainField2_)
{
  if(mainField2_.length()>500)  return false;
  delete mainField2;
  mainField2=0;

  mainField2=new std::string(mainField2_);
  return true;
}

void Show_request::reset_mainField2(void)
{
  if(mainField2)
    delete mainField2;
  mainField2=0;
}

bool Show_request::set_alignment(const TextAlignment& alignment_)
{
  if(!TextAlignmentMarshaller::checkIntegrityConst(alignment_))   return false;
  delete alignment;
  alignment=0;

  alignment=new TextAlignment(alignment_);
  return true;
}

void Show_request::reset_alignment(void)
{
  if(alignment)
    delete alignment;
  alignment=0;
}

bool Show_request::set_statusBar(const std::string& statusBar_)
{
  if(statusBar_.length()>500)  return false;
  delete statusBar;
  statusBar=0;

  statusBar=new std::string(statusBar_);
  return true;
}

void Show_request::reset_statusBar(void)
{
  if(statusBar)
    delete statusBar;
  statusBar=0;
}

bool Show_request::set_mediaClock(const std::string& mediaClock_)
{
  if(mediaClock_.length()>500)  return false;
  delete mediaClock;
  mediaClock=0;

  mediaClock=new std::string(mediaClock_);
  return true;
}

void Show_request::reset_mediaClock(void)
{
  if(mediaClock)
    delete mediaClock;
  mediaClock=0;
}

bool Show_request::set_mediaTrack(const std::string& mediaTrack_)
{
  if(mediaTrack_.length()>500)  return false;
  delete mediaTrack;
  mediaTrack=0;

  mediaTrack=new std::string(mediaTrack_);
  return true;
}

void Show_request::reset_mediaTrack(void)
{
  if(mediaTrack)
    delete mediaTrack;
  mediaTrack=0;
}




const std::string* Show_request::get_mainField1(void) const 
{
  return mainField1;
}

const std::string* Show_request::get_mainField2(void) const 
{
  return mainField2;
}

const TextAlignment* Show_request::get_alignment(void) const 
{
  return alignment;
}

const std::string* Show_request::get_statusBar(void) const 
{
  return statusBar;
}

const std::string* Show_request::get_mediaClock(void) const 
{
  return mediaClock;
}

const std::string* Show_request::get_mediaTrack(void) const 
{
  return mediaTrack;
}

