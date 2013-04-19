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

#include "../include/JSONHandler/SDLRPCObjects/V2/PerformAudioPassThru_request.h"
#include "PerformAudioPassThru_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "AudioCaptureQualityMarshaller.h"
#include "AudioTypeMarshaller.h"
#include "SamplingRateMarshaller.h"
#include "TTSChunkMarshaller.h"

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
PerformAudioPassThru_request& PerformAudioPassThru_request::operator =(const PerformAudioPassThru_request& c)
{
  initialPrompt= c.initialPrompt;
  audioPassThruDisplayText1= c.audioPassThruDisplayText1 ? new std::string(c.audioPassThruDisplayText1[0]) : 0;
  audioPassThruDisplayText2= c.audioPassThruDisplayText2 ? new std::string(c.audioPassThruDisplayText2[0]) : 0;
  samplingRate= c.samplingRate;
  maxDuration= c.maxDuration;
  bitsPerSample= c.bitsPerSample;
  audioType= c.audioType;

  return *this;
}


PerformAudioPassThru_request::~PerformAudioPassThru_request(void)
{
  if(audioPassThruDisplayText1)
    delete audioPassThruDisplayText1;
  if(audioPassThruDisplayText2)
    delete audioPassThruDisplayText2;
}


PerformAudioPassThru_request::PerformAudioPassThru_request(const PerformAudioPassThru_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool PerformAudioPassThru_request::checkIntegrity(void)
{
  return PerformAudioPassThru_requestMarshaller::checkIntegrity(*this);
}


PerformAudioPassThru_request::PerformAudioPassThru_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      audioPassThruDisplayText1(0),
    audioPassThruDisplayText2(0)
{
}



bool PerformAudioPassThru_request::set_initialPrompt(const std::vector<TTSChunk>& initialPrompt_)
{
  unsigned int i=initialPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(initialPrompt_[i]))   return false;
  }
  initialPrompt=initialPrompt_;
  return true;
}

bool PerformAudioPassThru_request::set_audioPassThruDisplayText1(const std::string& audioPassThruDisplayText1_)
{
  if(audioPassThruDisplayText1_.length()>500)  return false;
  delete audioPassThruDisplayText1;
  audioPassThruDisplayText1=0;

  audioPassThruDisplayText1=new std::string(audioPassThruDisplayText1_);
  return true;
}

void PerformAudioPassThru_request::reset_audioPassThruDisplayText1(void)
{
  if(audioPassThruDisplayText1)
    delete audioPassThruDisplayText1;
  audioPassThruDisplayText1=0;
}

bool PerformAudioPassThru_request::set_audioPassThruDisplayText2(const std::string& audioPassThruDisplayText2_)
{
  if(audioPassThruDisplayText2_.length()>500)  return false;
  delete audioPassThruDisplayText2;
  audioPassThruDisplayText2=0;

  audioPassThruDisplayText2=new std::string(audioPassThruDisplayText2_);
  return true;
}

void PerformAudioPassThru_request::reset_audioPassThruDisplayText2(void)
{
  if(audioPassThruDisplayText2)
    delete audioPassThruDisplayText2;
  audioPassThruDisplayText2=0;
}

bool PerformAudioPassThru_request::set_samplingRate(const SamplingRate& samplingRate_)
{
  if(!SamplingRateMarshaller::checkIntegrityConst(samplingRate_))   return false;
  samplingRate=samplingRate_;
  return true;
}

bool PerformAudioPassThru_request::set_maxDuration(unsigned int maxDuration_)
{
  if(maxDuration_>1000000)  return false;
  maxDuration=maxDuration_;
  return true;
}

bool PerformAudioPassThru_request::set_bitsPerSample(const AudioCaptureQuality& bitsPerSample_)
{
  if(!AudioCaptureQualityMarshaller::checkIntegrityConst(bitsPerSample_))   return false;
  bitsPerSample=bitsPerSample_;
  return true;
}

bool PerformAudioPassThru_request::set_audioType(const AudioType& audioType_)
{
  if(!AudioTypeMarshaller::checkIntegrityConst(audioType_))   return false;
  audioType=audioType_;
  return true;
}




const std::vector<TTSChunk>& PerformAudioPassThru_request::get_initialPrompt(void) const 
{
  return initialPrompt;
}

const std::string* PerformAudioPassThru_request::get_audioPassThruDisplayText1(void) const 
{
  return audioPassThruDisplayText1;
}

const std::string* PerformAudioPassThru_request::get_audioPassThruDisplayText2(void) const 
{
  return audioPassThruDisplayText2;
}

const SamplingRate& PerformAudioPassThru_request::get_samplingRate(void) const 
{
  return samplingRate;
}

unsigned int PerformAudioPassThru_request::get_maxDuration(void) const
{
  return maxDuration;
}

const AudioCaptureQuality& PerformAudioPassThru_request::get_bitsPerSample(void) const 
{
  return bitsPerSample;
}

const AudioType& PerformAudioPassThru_request::get_audioType(void) const 
{
  return audioType;
}

